#include "calc/renal.hpp"

#include <cmath>
#include <utility>

#include "calc/text.hpp"

namespace calc {

RenalFunction RenalFunction::assumedNormal() {
  return RenalFunction{};
}

RenalFunction RenalFunction::entered(Gfr gfr) {
  RenalFunction renal;
  renal.source = RenalSource::EnteredDirectly;
  renal.gfr = gfr;
  return renal;
}

RenalFunction RenalFunction::estimated(Gfr gfr, std::string method) {
  RenalFunction renal;
  renal.source = RenalSource::Estimated;
  renal.gfr = gfr;
  renal.method = std::move(method);
  return renal;
}

std::vector<Warning> validateGfr(Gfr gfr) {
  std::vector<Warning> warnings;
  if (!(gfr.value() > 0.0) || gfr.value() > limits::kMaxPlausibleGfr) {
    warnings.push_back({Severity::Blocking, "TFG_INVALIDA",
                        "La TFG debe ser mayor que 0 y no superar " +
                            formatNumber(limits::kMaxPlausibleGfr) + " mL/min/1,73 m²."});
  }
  return warnings;
}

// --- Comprobaciones comunes a las fórmulas -----------------------------------

namespace {

void requireHeight(const Patient& patient, GfrEstimate& estimate) {
  if (!patient.height) {
    estimate.warnings.push_back(
        {Severity::Blocking, "TALLA_REQUERIDA",
         "Para estimar la TFG se necesita la talla del paciente en centímetros."});
  }
}

void requireValidCreatinine(MgPerDl serum_creatinine, GfrEstimate& estimate) {
  const double creatinine = serum_creatinine.value();
  if (!(creatinine > 0.0) || creatinine > limits::kMaxPlausibleCreatinineMgDl) {
    estimate.warnings.push_back(
        {Severity::Blocking, "CREATININA_INVALIDA",
         "La creatinina sérica debe ser mayor que 0 y no superar " +
             formatNumber(limits::kMaxPlausibleCreatinineMgDl) +
             " mg/dL. Verifique el valor y la unidad."});
  }
}

}  // namespace

// --- Schwartz de cabecera ----------------------------------------------------

std::string BedsideSchwartzEstimator::name() const {
  return "Schwartz de cabecera";
}

GfrEstimate BedsideSchwartzEstimator::estimate(const Patient& patient,
                                               MgPerDl serum_creatinine) const {
  GfrEstimate estimate;
  requireHeight(patient, estimate);
  requireValidCreatinine(serum_creatinine, estimate);
  if (hasBlocking(estimate.warnings)) {
    return estimate;
  }

  const double creatinine = serum_creatinine.value();
  const double height_cm = patient.height->value();
  const double gfr = kCoefficient * height_cm / creatinine;
  estimate.gfr = Gfr(gfr);
  estimate.formula = formatNumber(kCoefficient, 3) + " × " + formatNumber(height_cm, 1) +
                     " cm ÷ " + formatNumber(creatinine) + " mg/dL = " + formatNumber(gfr, 1) +
                     " mL/min/1,73 m²";

  const int months = patient.age.totalMonths();
  if (months < kValidatedMinAgeMonths || months >= kValidatedMaxAgeMonthsExclusive) {
    estimate.warnings.push_back(
        {Severity::Caution, "SCHWARTZ_FUERA_DE_RANGO_DE_EDAD",
         "La fórmula de Schwartz se derivó en niños de 1 a 16 años; en esta edad la "
         "estimación es menos confiable."});
  }
  return estimate;
}

// --- CKiD U25 ----------------------------------------------------------------

namespace {

/// Partes de la constante k de CKiD U25: k = base × crecimiento^(edad − 12),
/// o un valor fijo desde los 18 años (crecimiento = 1).
struct U25Coefficient {
  double base;
  double growth;
  bool fixed;
};

constexpr double kU25PivotAgeYears = 12.0;
constexpr double kU25AdultAgeYears = 18.0;

U25Coefficient u25Coefficient(double age_years, Sex sex) noexcept {
  const bool male = sex == Sex::Male;
  if (age_years >= kU25AdultAgeYears) {
    return {male ? 50.8 : 41.4, 1.0, true};
  }
  const double base = male ? 39.0 : 36.1;
  if (age_years < kU25PivotAgeYears) {
    return {base, 1.008, false};
  }
  return {base, male ? 1.045 : 1.023, false};
}

double valueOf(const U25Coefficient& k, double age_years) noexcept {
  return k.fixed ? k.base : k.base * std::pow(k.growth, age_years - kU25PivotAgeYears);
}

}  // namespace

double CkidU25Estimator::coefficient(Age age, Sex sex) noexcept {
  return valueOf(u25Coefficient(age.years(), sex), age.years());
}

std::string CkidU25Estimator::name() const {
  return "CKiD U25";
}

GfrEstimate CkidU25Estimator::estimate(const Patient& patient, MgPerDl serum_creatinine) const {
  GfrEstimate estimate;
  requireHeight(patient, estimate);
  if (!patient.sex) {
    estimate.warnings.push_back(
        {Severity::Blocking, "SEXO_REQUERIDO",
         "La ecuación CKiD U25 usa constantes distintas por sexo: registre el sexo del "
         "paciente o elija la fórmula de Schwartz."});
  }
  requireValidCreatinine(serum_creatinine, estimate);
  if (hasBlocking(estimate.warnings)) {
    return estimate;
  }

  const double creatinine = serum_creatinine.value();
  const double height_m = patient.height->value() / 100.0;
  const double age_years = patient.age.years();
  const U25Coefficient parts = u25Coefficient(age_years, *patient.sex);
  const double k = valueOf(parts, age_years);
  const double gfr = k * height_m / creatinine;
  estimate.gfr = Gfr(gfr);

  // Desglose: primero cómo sale k, luego la TFG.
  std::string k_text = "k (" + std::string(label(*patient.sex)) + ", " + patient.age.describe() + ") = ";
  if (parts.fixed) {
    k_text += formatNumber(k, 1);
  } else {
    k_text += formatNumber(parts.base, 1) + " × " + formatNumber(parts.growth, 3) + "^(" +
              formatNumber(age_years) + " − 12) = " + formatNumber(k);
  }
  estimate.formula = k_text + "; TFG = " + formatNumber(k) + " × " + formatNumber(height_m, 3) +
                     " m ÷ " + formatNumber(creatinine) + " mg/dL = " + formatNumber(gfr, 1) +
                     " mL/min/1,73 m²";

  const int months = patient.age.totalMonths();
  if (months < kValidatedMinAgeMonths || months >= kValidatedMaxAgeMonthsExclusive) {
    estimate.warnings.push_back(
        {Severity::Caution, "CKID_U25_FUERA_DE_RANGO_DE_EDAD",
         "La ecuación CKiD U25 se derivó en personas de 1 a 25 años; en esta edad la "
         "estimación es menos confiable."});
  }
  return estimate;
}

// --- Catálogo de fórmulas ----------------------------------------------------

std::string_view id(GfrFormula formula) noexcept {
  switch (formula) {
    case GfrFormula::CkidU25:
      return "ckid-u25";
    case GfrFormula::BedsideSchwartz:
      return "schwartz";
  }
  return "";
}

std::optional<GfrFormula> gfrFormulaFromId(std::string_view formula_id) noexcept {
  for (const GfrFormula formula : kGfrFormulas) {
    if (id(formula) == formula_id) {
      return formula;
    }
  }
  return std::nullopt;
}

std::unique_ptr<GfrEstimator> makeGfrEstimator(GfrFormula formula) {
  switch (formula) {
    case GfrFormula::CkidU25:
      return std::make_unique<CkidU25Estimator>();
    case GfrFormula::BedsideSchwartz:
      return std::make_unique<BedsideSchwartzEstimator>();
  }
  return nullptr;
}

}  // namespace calc
