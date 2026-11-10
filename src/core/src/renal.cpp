#include "calc/renal.hpp"

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

// --- Schwartz de cabecera ----------------------------------------------------

std::string BedsideSchwartzEstimator::name() const {
  return "Schwartz de cabecera";
}

GfrEstimate BedsideSchwartzEstimator::estimate(const Patient& patient,
                                               MgPerDl serum_creatinine) const {
  GfrEstimate estimate;

  if (!patient.height) {
    estimate.warnings.push_back(
        {Severity::Blocking, "TALLA_REQUERIDA",
         "La fórmula de Schwartz necesita la talla del paciente en centímetros."});
  }
  const double creatinine = serum_creatinine.value();
  if (!(creatinine > 0.0) || creatinine > limits::kMaxPlausibleCreatinineMgDl) {
    estimate.warnings.push_back(
        {Severity::Blocking, "CREATININA_INVALIDA",
         "La creatinina sérica debe ser mayor que 0 y no superar " +
             formatNumber(limits::kMaxPlausibleCreatinineMgDl) +
             " mg/dL. Verifique el valor y la unidad."});
  }
  if (hasBlocking(estimate.warnings)) {
    return estimate;
  }

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

}  // namespace calc
