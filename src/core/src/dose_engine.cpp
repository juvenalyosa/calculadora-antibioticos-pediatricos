#include "calc/dose_engine.hpp"

#include <algorithm>
#include <cmath>

#include "calc/text.hpp"

namespace calc {

namespace {

/// Tolerancia relativa para comparar números de punto flotante con un tope.
constexpr double kRelativeTolerance = 1e-9;

bool exceeds(double value, double limit) noexcept {
  return value > limit * (1.0 + kRelativeTolerance);
}

void addStep(RegimenResult& result, std::string title, std::string detail) {
  result.trace.push_back({std::move(title), std::move(detail)});
}

void addWarning(RegimenResult& result, Severity severity, std::string code, std::string message) {
  result.warnings.push_back({severity, std::move(code), std::move(message)});
}

/// Unidad de la dosis tal como la escribe la guía: "mg/kg/día", "UI/kg/dosis"...
std::string basisUnit(DoseBasis basis, DrugUnit unit) {
  const std::string drug_unit(symbol(unit));
  switch (basis) {
    case DoseBasis::PerKgPerDay:
      return drug_unit + "/kg/día";
    case DoseBasis::PerKgPerDose:
      return drug_unit + "/kg/dosis";
    case DoseBasis::Fixed:
    case DoseBasis::NoCalculation:
      return drug_unit;
  }
  return drug_unit;
}

/// Dosis por toma ANTES de topes y ajustes, para un valor de dosis de la guía.
double basePerDose(const Regimen& regimen, double dose_value, double weight_kg) noexcept {
  switch (regimen.basis) {
    case DoseBasis::PerKgPerDay:
      // El cargador garantiza que mg/kg/día siempre trae intervalo.
      return dose_value * weight_kg * (*regimen.interval_hours) / 24.0;
    case DoseBasis::PerKgPerDose:
      return dose_value * weight_kg;
    case DoseBasis::Fixed:
      return dose_value;
    case DoseBasis::NoCalculation:
      return 0.0;
  }
  return 0.0;
}

struct CappedDose {
  double per_dose = 0.0;
  bool capped_by_dose = false;
  bool capped_by_day = false;
};

/// Aplica el tope por toma y luego el tope diario.
CappedDose applyCaps(const Regimen& regimen, double per_dose) noexcept {
  CappedDose capped{per_dose, false, false};
  if (regimen.max_per_dose && exceeds(capped.per_dose, *regimen.max_per_dose)) {
    capped.per_dose = *regimen.max_per_dose;
    capped.capped_by_dose = true;
  }
  if (regimen.max_per_day && regimen.interval_hours) {
    const double daily = capped.per_dose * 24.0 / *regimen.interval_hours;
    if (exceeds(daily, *regimen.max_per_day)) {
      capped.per_dose = *regimen.max_per_day * (*regimen.interval_hours) / 24.0;
      capped.capped_by_day = true;
    }
  }
  return capped;
}

/// Motivo por el que un esquema no aplica al paciente; vacío si aplica.
/// Los rangos son semiabiertos: [mínimo, máximo).
std::optional<std::string> inapplicableReason(const Patient& patient, const Regimen& regimen) {
  const int months = patient.age.totalMonths();
  const double kg = patient.weight.value();
  if (regimen.age_min_months && months < *regimen.age_min_months) {
    return "requiere edad de al menos " + Age::fromMonths(*regimen.age_min_months).describe();
  }
  if (regimen.age_max_months && months >= *regimen.age_max_months) {
    return "solo para menores de " + Age::fromMonths(*regimen.age_max_months).describe();
  }
  if (regimen.weight_min_kg && kg < *regimen.weight_min_kg) {
    return "requiere peso de al menos " + formatNumber(*regimen.weight_min_kg) + " kg";
  }
  if (regimen.weight_max_kg && kg >= *regimen.weight_max_kg) {
    return "solo para peso menor de " + formatNumber(*regimen.weight_max_kg) + " kg";
  }
  return std::nullopt;
}

// --- Paso 1: dosis base ------------------------------------------------------

double computeBaseDose(const Regimen& regimen, const Drug& drug, double weight_kg,
                       RegimenResult& result) {
  const double per_dose = basePerDose(regimen, regimen.dose_usual, weight_kg);
  const std::string guide_dose =
      formatNumber(regimen.dose_usual) + " " + basisUnit(regimen.basis, drug.unit);

  switch (regimen.basis) {
    case DoseBasis::PerKgPerDay: {
      const double daily = regimen.dose_usual * weight_kg;
      const int interval = *regimen.interval_hours;
      addStep(result, "Dosis diaria",
              guide_dose + " × " + formatNumber(weight_kg) + " kg = " +
                  formatAmount(daily, drug.unit) + "/día");
      if (24 % interval == 0) {
        const int doses = 24 / interval;
        addStep(result, "Dosis por toma",
                formatAmount(daily, drug.unit) + "/día ÷ " + std::to_string(doses) +
                    (doses == 1 ? " toma" : " tomas") + " (cada " + std::to_string(interval) +
                    " h) = " + formatAmount(per_dose, drug.unit));
      } else {
        addStep(result, "Dosis por toma",
                formatAmount(daily, drug.unit) + "/día × " + std::to_string(interval) +
                    " h ÷ 24 h = " + formatAmount(per_dose, drug.unit));
      }
      break;
    }
    case DoseBasis::PerKgPerDose:
      addStep(result, "Dosis por toma",
              guide_dose + " × " + formatNumber(weight_kg) + " kg = " +
                  formatAmount(per_dose, drug.unit));
      break;
    case DoseBasis::Fixed:
      addStep(result, "Dosis fija",
              formatAmount(per_dose, drug.unit) + " por toma (no depende del peso)");
      break;
    case DoseBasis::NoCalculation:
      break;
  }
  return per_dose;
}

// --- Paso 2: topes máximos ---------------------------------------------------

double applyCapsWithTrace(const Regimen& regimen, const Drug& drug, double per_dose,
                          RegimenResult& result) {
  const CappedDose capped = applyCaps(regimen, per_dose);

  if (capped.capped_by_dose) {
    addStep(result, "Tope por toma",
            formatAmount(per_dose, drug.unit) + " supera el máximo de " +
                formatAmount(*regimen.max_per_dose, drug.unit) + " por toma; se usa " +
                formatAmount(*regimen.max_per_dose, drug.unit) + ".");
  }
  if (capped.capped_by_day) {
    const double uncapped_daily = (capped.capped_by_dose ? *regimen.max_per_dose : per_dose) *
                                  24.0 / *regimen.interval_hours;
    addStep(result, "Tope diario",
            formatAmount(uncapped_daily, drug.unit) + "/día supera el máximo de " +
                formatAmount(*regimen.max_per_day, drug.unit) + "/día; se usa " +
                formatAmount(*regimen.max_per_day, drug.unit) + "/día (" +
                formatAmount(capped.per_dose, drug.unit) + " por toma).");
  }
  if (capped.capped_by_dose || capped.capped_by_day) {
    result.max_cap_applied = true;
    addWarning(result, Severity::Info, "TOPE_MAXIMO_APLICADO",
               "La dosis calculada por peso superaba el máximo; se aplicó el tope de la guía.");
  }

  // Sin tope registrado, la dosis por kilogramo crece con el peso sin límite:
  // en un adolescente puede superar la del adulto. Se avisa siempre.
  const bool weight_based =
      regimen.basis == DoseBasis::PerKgPerDay || regimen.basis == DoseBasis::PerKgPerDose;
  if (weight_based && !regimen.max_per_dose && !regimen.max_per_day) {
    addWarning(result, Severity::Caution, "SIN_TOPE_MAXIMO",
               "La fuente no define una dosis máxima para este esquema. En pacientes de mayor "
               "peso la dosis por kilogramo puede superar la del adulto: verifíquela.");
  }
  return capped.per_dose;
}

// --- Paso 3: ajuste renal ----------------------------------------------------

void applyRenalAdjustment(const RenalFunction& renal, const Drug& drug, double weight_kg,
                          double& per_dose, std::optional<int>& interval, RegimenResult& result) {
  if (!renal.gfr) {
    return;  // Función renal no evaluada: se informa a nivel de plan.
  }
  const double gfr = renal.gfr->value();
  const std::string gfr_text = "TFG " + formatNumber(gfr, 1) + " mL/min/1,73 m²";
  const RenalAdjustment& adjustment = drug.renal;
  const std::string drug_note = adjustment.note.empty() ? "" : " " + adjustment.note;

  if (adjustment.kind == RenalAdjustmentKind::NoData) {
    if (gfr < limits::kReducedGfrThreshold) {
      addStep(result, "Ajuste renal",
              gfr_text + ": la base de conocimiento aún no tiene el ajuste de " + drug.name + "." +
                  drug_note);
      addWarning(result, Severity::Caution, "SIN_DATO_AJUSTE_RENAL",
                 "No hay información de ajuste renal para " + drug.name +
                     " en la base de conocimiento. La dosis mostrada NO está ajustada; "
                     "consulte una referencia farmacológica." +
                     drug_note);
    }
    return;
  }

  if (adjustment.status != ReviewStatus::Verified) {
    addWarning(result, Severity::Caution, "AJUSTE_RENAL_NO_VERIFICADO",
               "La información de ajuste renal de " + drug.name + " está en estado «" +
                   std::string(label(adjustment.status)) + "»: no ha sido verificada.");
  }

  if (adjustment.kind == RenalAdjustmentKind::NotRequired) {
    addStep(result, "Ajuste renal",
            drug.name + " no requiere ajuste por función renal." + drug_note);
    return;
  }

  const RenalBand* band = adjustment.findBand(gfr);
  if (band == nullptr) {
    addStep(result, "Ajuste renal", gfr_text + ": fuera de las bandas de ajuste; sin cambio.");
    return;
  }

  const std::string band_text = gfr_text + " (banda " + formatNumber(band->gfr_min) + " a <" +
                                formatNumber(band->gfr_max) + ")";
  const std::string note = band->note.empty() ? "" : " " + band->note;

  if (band->avoid) {
    result.not_recommended = true;
    addStep(result, "Ajuste renal", band_text + ": evitar " + drug.name + "." + note);
    addWarning(result, Severity::Blocking, "EVITAR_POR_FUNCION_RENAL",
               drug.name + " debe evitarse con esta función renal." + note);
    return;
  }

  // La aclaración de la banda se muestra siempre, cambie o no la dosis.
  if (!band->note.empty()) {
    addWarning(result, Severity::Caution, "NOTA_AJUSTE_RENAL", band->note);
  }

  // Orden: dosis de reemplazo (si la banda la define), fracción, tope de la banda.
  double adjusted_dose = per_dose;
  std::string detail = band_text + ": ";
  if (band->replacement_dose_per_kg) {
    adjusted_dose = *band->replacement_dose_per_kg * weight_kg;
    detail += "dosis de reemplazo " + formatNumber(*band->replacement_dose_per_kg) + " " +
              std::string(symbol(drug.unit)) + "/kg × " + formatNumber(weight_kg) + " kg = " +
              formatAmount(adjusted_dose, drug.unit) + " por toma; ";
  }
  if (band->dose_factor != 1.0) {
    adjusted_dose *= band->dose_factor;
    detail += "dosis por toma × " + formatNumber(band->dose_factor) + "; ";
  }
  if (band->max_per_dose && exceeds(adjusted_dose, *band->max_per_dose)) {
    adjusted_dose = *band->max_per_dose;
    detail += "tope de " + formatAmount(*band->max_per_dose, drug.unit) + " por toma; ";
  }

  std::optional<int> adjusted_interval = interval;
  if (interval && band->min_interval_hours) {
    // Se toma el mayor: un ajuste renal nunca aumenta la frecuencia.
    adjusted_interval = std::max(*interval, *band->min_interval_hours);
    detail += "intervalo mínimo " + std::to_string(*band->min_interval_hours) + " h; ";
  }

  const bool changed = adjusted_dose != per_dose || adjusted_interval != interval;
  if (!changed) {
    addStep(result, "Ajuste renal", band_text + ": el esquema ya cumple el ajuste; sin cambio." + note);
    return;
  }

  detail += "resultado: " + formatAmount(adjusted_dose, drug.unit) + " " +
            describeInterval(adjusted_interval) + "." + note + drug_note;
  addStep(result, "Ajuste renal", detail);
  addWarning(result, Severity::Info, "AJUSTE_RENAL_APLICADO",
             "La dosis se ajustó por función renal (" + gfr_text + ").");

  result.renal_adjustment_applied = true;
  per_dose = adjusted_dose;
  interval = adjusted_interval;
}

// --- Paso 4: conversión a presentación ---------------------------------------

Administration administer(const Presentation& presentation, const Regimen& regimen, const Drug& drug,
                          double per_dose, RegimenResult& result) {
  Administration administration;
  administration.presentation_id = presentation.id;
  administration.presentation = presentation.description;
  administration.dispensing_unit = presentation.dispensing_unit;

  // Regla de tres: cantidad = dosis ÷ concentración.
  const double step = presentation.rounding_step;
  administration.exact_quantity = per_dose / presentation.amount_per_unit;
  double rounded = std::round(administration.exact_quantity / step) * step;

  // El redondeo hacia arriba no puede sobrepasar el tope por toma.
  if (regimen.max_per_dose &&
      exceeds(rounded * presentation.amount_per_unit, *regimen.max_per_dose)) {
    rounded = std::floor(administration.exact_quantity / step + kRelativeTolerance) * step;
  }

  administration.rounded_quantity = rounded;
  administration.delivered_amount = rounded * presentation.amount_per_unit;
  administration.deviation = (administration.delivered_amount - per_dose) / per_dose;
  administration.suitable =
      rounded > 0.0 &&
      std::abs(administration.deviation) <= limits::kMaxRoundingDeviation + kRelativeTolerance;

  std::string detail = formatAmount(per_dose, drug.unit) + " ÷ " +
                       formatAmount(presentation.amount_per_unit, drug.unit) + "/" +
                       presentation.dispensing_unit + " = " +
                       describeQuantity(administration.exact_quantity,
                                        presentation.dispensing_unit);
  if (std::abs(rounded - administration.exact_quantity) > 1e-6) {
    const double percent = administration.deviation * 100.0;
    detail += " → se redondea a " + describeAdministration(administration) + " (entrega " +
              formatAmount(administration.delivered_amount, drug.unit) + ", " +
              (percent > 0 ? "+" : "") + formatNumber(percent, 1) + " %)";
  }
  if (!administration.suitable) {
    detail += ". Presentación no adecuada para esta dosis.";
  }
  addStep(result, presentation.description, detail);
  return administration;
}

void computeAdministrations(const Regimen& regimen, const Drug& drug, double per_dose,
                            RegimenResult& result) {
  for (const Presentation& presentation : drug.presentations) {
    if (presentation.route == regimen.route) {
      result.administrations.push_back(administer(presentation, regimen, drug, per_dose, result));
    }
  }

  if (result.administrations.empty()) {
    addWarning(result, Severity::Info, "SIN_PRESENTACION_REGISTRADA",
               "No hay presentaciones de " + drug.name + " por " + std::string(label(regimen.route)) +
                   " en la base de conocimiento.");
    return;
  }
  const bool any_suitable =
      std::any_of(result.administrations.begin(), result.administrations.end(),
                  [](const Administration& a) { return a.suitable; });
  if (!any_suitable) {
    addWarning(result, Severity::Caution, "SIN_PRESENTACION_ADECUADA",
               "Ninguna presentación registrada permite administrar esta dosis con una "
               "desviación menor del " +
                   formatNumber(limits::kMaxRoundingDeviation * 100.0) + " %.");
  }
}

}  // namespace

// =============================================================================
// DoseEngine
// =============================================================================

RegimenResult DoseEngine::calculate(const Patient& patient, const RenalFunction& renal,
                                    const Regimen& regimen) const {
  const Drug& drug = kb_.drug(regimen.drug_id);

  RegimenResult result;
  result.drug_id = drug.id;
  result.drug_name = drug.name;
  result.unit = drug.unit;
  result.route = regimen.route;
  result.duration_min_days = regimen.duration_min_days;
  result.duration_max_days = regimen.duration_max_days;
  result.note = regimen.note;
  result.status = regimen.status;

  result.source.id = regimen.source_id;
  result.source.page = regimen.page;
  if (const Source* source = kb_.findSource(regimen.source_id)) {
    result.source.title = source->title;
    result.source.organization = source->organization;
    result.source.year = source->year;
  }

  if (regimen.status != ReviewStatus::Verified) {
    addWarning(result, Severity::Caution, "DATOS_NO_VERIFICADOS",
               "Esquema en estado «" + std::string(label(regimen.status)) +
                   "»: no ha sido verificado clínicamente. No usar para decisiones clínicas.");
  }

  if (regimen.basis == DoseBasis::NoCalculation) {
    addStep(result, "Indicación sin cálculo", "Este esquema no se calcula por peso: " + regimen.note);
    return result;
  }
  result.calculated = true;

  const double weight_kg = patient.weight.value();

  // 1) Dosis base según la guía.
  double per_dose = computeBaseDose(regimen, drug, weight_kg, result);

  // 2) Topes máximos.
  per_dose = applyCapsWithTrace(regimen, drug, per_dose, result);

  // Rango de la guía (mismos pasos 1 y 2 con los extremos), como referencia.
  if (regimen.dose_min != regimen.dose_max) {
    result.range_min_per_dose =
        applyCaps(regimen, basePerDose(regimen, regimen.dose_min, weight_kg)).per_dose;
    result.range_max_per_dose =
        applyCaps(regimen, basePerDose(regimen, regimen.dose_max, weight_kg)).per_dose;
    // Un tope puede igualar los dos extremos: entonces se dice una sola cifra.
    const bool capped_flat =
        std::abs(*result.range_max_per_dose - *result.range_min_per_dose) < 1e-9;
    addStep(result, "Rango de la guía",
            formatNumber(regimen.dose_min) + " a " + formatNumber(regimen.dose_max) + " " +
                basisUnit(regimen.basis, drug.unit) + " → " +
                (capped_flat
                     ? formatAmount(*result.range_max_per_dose, drug.unit) +
                           " por toma en todo el rango, por el tope máximo"
                     : formatAmount(*result.range_min_per_dose, drug.unit) + " a " +
                           formatAmount(*result.range_max_per_dose, drug.unit) + " por toma"));
  }

  // 3) Ajuste renal.
  std::optional<int> interval = regimen.interval_hours;
  applyRenalAdjustment(renal, drug, weight_kg, per_dose, interval, result);

  result.dose_per_administration = per_dose;
  result.interval_hours = interval;
  if (interval) {
    result.daily_dose = per_dose * 24.0 / *interval;
  }

  // 4) Conversión a cada presentación compatible con la vía.
  if (!result.not_recommended) {
    computeAdministrations(regimen, drug, per_dose, result);
  }
  return result;
}

TreatmentPlan DoseEngine::plan(const CalculationRequest& request) const {
  const Disease& disease = kb_.disease(request.disease_id);

  TreatmentPlan plan;
  plan.disease_id = disease.id;
  plan.disease_name = disease.name;
  plan.kb_version = kb_.version;
  plan.antibiotic_indicated = disease.antibiotic_indicated;
  plan.message = disease.message;

  // Validación de entradas: si algo es bloqueante no se calcula nada.
  plan.warnings = validatePatient(request.patient);
  if (request.renal.gfr) {
    const std::vector<Warning> renal_warnings = validateGfr(*request.renal.gfr);
    plan.warnings.insert(plan.warnings.end(), renal_warnings.begin(), renal_warnings.end());
  }
  if (hasBlocking(plan.warnings)) {
    plan.blocked = true;
    return plan;
  }

  if (!disease.antibiotic_indicated) {
    return plan;
  }

  if (!request.renal.gfr) {
    plan.warnings.push_back(
        {Severity::Info, "FUNCION_RENAL_NO_EVALUADA",
         "No se registró la función renal: las dosis se calcularon sin ajuste renal."});
  }

  for (const TreatmentLine& line : disease.lines) {
    LineResult line_result;
    line_result.kind = line.kind;

    for (const TherapyOption& option : line.options) {
      // Una opción aplica solo si TODOS sus componentes aplican al paciente.
      std::optional<std::string> reason;
      for (const Regimen& regimen : option.components) {
        reason = inapplicableReason(request.patient, regimen);
        if (reason) {
          break;
        }
      }
      if (reason) {
        line_result.skipped.push_back({option.number, option.description, *reason});
        continue;
      }

      OptionResult option_result;
      option_result.number = option.number;
      option_result.description = option.description;
      for (const Regimen& regimen : option.components) {
        option_result.components.push_back(calculate(request.patient, request.renal, regimen));
      }
      line_result.options.push_back(std::move(option_result));
    }

    plan.lines.push_back(std::move(line_result));
  }
  return plan;
}

}  // namespace calc
