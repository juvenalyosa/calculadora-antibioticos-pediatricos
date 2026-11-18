// Pruebas del motor con la base SINTÉTICA (tests/support/test_kb.hpp).
// Los valores esperados están calculados a mano en los comentarios.

#include <doctest/doctest.h>

#include <string>
#include <string_view>

#include "calc/dose_engine.hpp"
#include "support/test_kb.hpp"

using namespace calc;

namespace {

const LineResult& lineOf(const TreatmentPlan& plan, TreatmentLineKind kind) {
  for (const LineResult& line : plan.lines) {
    if (line.kind == kind) {
      return line;
    }
  }
  FAIL("el plan no contiene la línea pedida");
  return plan.lines.front();
}
// Devuelve una referencia al interior del plan: prohibir planes temporales
// evita referencias colgantes del tipo lineOf(planFor(...), ...).
const LineResult& lineOf(TreatmentPlan&&, TreatmentLineKind) = delete;

const OptionResult& optionOf(const LineResult& line, int number) {
  for (const OptionResult& option : line.options) {
    if (option.number == number) {
      return option;
    }
  }
  FAIL("la línea no contiene la opción " << number);
  return line.options.front();
}

// `id` se recibe por valor (string_view) y no como const std::string&: así no
// se crea un std::string temporal en la llamada, que GCC señalaría como una
// posible referencia colgante en el valor devuelto.
const Administration& administrationOf(const RegimenResult& result, std::string_view id) {
  for (const Administration& administration : result.administrations) {
    if (administration.presentation_id == id) {
      return administration;
    }
  }
  FAIL("no hay administración para la presentación " << std::string(id));
  return result.administrations.front();
}

TreatmentPlan planFor(const KnowledgeBase& kb, const Patient& patient,
                      const RenalFunction& renal = RenalFunction::assumedNormal()) {
  return DoseEngine(kb).plan({patient, renal, "enfermedad_x"});
}

}  // namespace

TEST_CASE("motor: mg/kg/día en el lactante (caso tipo 1)") {
  const KnowledgeBase kb = testing::loadTestKb();
  const TreatmentPlan plan = planFor(kb, testing::infantCase());
  REQUIRE_FALSE(plan.blocked);
  CHECK(hasCode(plan.warnings, "FUNCION_RENAL_NO_EVALUADA"));

  const RegimenResult& result =
      optionOf(lineOf(plan, TreatmentLineKind::First), 1).components.at(0);

  // 50 mg/kg/día × 10 kg = 500 mg/día; cada 8 h → 166,67 mg por toma.
  CHECK(result.calculated);
  CHECK(result.dose_per_administration == doctest::Approx(500.0 / 3.0));
  CHECK(result.interval_hours == 8);
  CHECK(result.daily_dose.value() == doctest::Approx(500.0));
  CHECK_FALSE(result.max_cap_applied);
  CHECK_FALSE(result.renal_adjustment_applied);

  // Rango 40–60 mg/kg/día → 133,33 a 200 mg por toma.
  CHECK(result.range_min_per_dose.value() == doctest::Approx(400.0 / 3.0));
  CHECK(result.range_max_per_dose.value() == doctest::Approx(200.0));

  // Suspensión 50 mg/mL: 3,33 mL → 3,5 mL (175 mg, +5 %): adecuada.
  const Administration& suspension = administrationOf(result, "a_susp");
  CHECK(suspension.exact_quantity == doctest::Approx(10.0 / 3.0));
  CHECK(suspension.rounded_quantity == doctest::Approx(3.5));
  CHECK(suspension.delivered_amount == doctest::Approx(175.0));
  CHECK(suspension.deviation == doctest::Approx(0.05));
  CHECK(suspension.suitable);

  // Tableta 500 mg ranurada: media tableta = 250 mg (+50 %): NO adecuada.
  const Administration& tablet = administrationOf(result, "a_tab");
  CHECK(tablet.rounded_quantity == doctest::Approx(0.5));
  CHECK_FALSE(tablet.suitable);

  // Dato verificado y con fuente: sin advertencia de "no verificado".
  CHECK_FALSE(hasCode(result.warnings, "DATOS_NO_VERIFICADOS"));
  CHECK(result.source.title == "Guía de prueba");
  CHECK(result.source.page == "Tabla 1");
  CHECK(result.duration_min_days == 7);
  CHECK(result.duration_max_days == 10);
  CHECK_FALSE(result.trace.empty());
}

TEST_CASE("motor: tope diario en el adolescente (caso tipo 2)") {
  const KnowledgeBase kb = testing::loadTestKb();
  const TreatmentPlan plan = planFor(kb, testing::adolescentCase());
  const RegimenResult& result =
      optionOf(lineOf(plan, TreatmentLineKind::First), 1).components.at(0);

  // 50 × 50 = 2500 mg/día > tope 1500 mg/día → 500 mg cada 8 h.
  CHECK(result.dose_per_administration == doctest::Approx(500.0));
  CHECK(result.daily_dose.value() == doctest::Approx(1500.0));
  CHECK(result.max_cap_applied);
  CHECK(hasCode(result.warnings, "TOPE_MAXIMO_APLICADO"));

  // El rango también respeta el tope.
  CHECK(result.range_min_per_dose.value() == doctest::Approx(500.0));
  CHECK(result.range_max_per_dose.value() == doctest::Approx(500.0));
  // Con los dos extremos iguales el desglose da una sola cifra, no «500 mg a 500 mg».
  bool flat_range_explained = false;
  for (const auto& step : result.trace) {
    if (step.detail.find("500 mg por toma en todo el rango") != std::string::npos) {
      flat_range_explained = true;
    }
    CHECK(step.detail.find("500 mg a 500 mg") == std::string::npos);
  }
  CHECK(flat_range_explained);

  CHECK(administrationOf(result, "a_susp").rounded_quantity == doctest::Approx(10.0));
  CHECK(administrationOf(result, "a_tab").rounded_quantity == doctest::Approx(1.0));
  CHECK(administrationOf(result, "a_tab").suitable);
}

TEST_CASE("motor: tope por toma y terapia combinada") {
  const KnowledgeBase kb = testing::loadTestKb();
  const TreatmentPlan plan = planFor(kb, testing::adolescentCase());
  const OptionResult& option = optionOf(lineOf(plan, TreatmentLineKind::Second), 1);
  REQUIRE(option.components.size() == 2);

  // Fármaco A: 25 mg/kg/dosis × 50 kg = 1250 mg > tope 500 mg por toma.
  const RegimenResult& drug_a = option.components[0];
  CHECK(drug_a.dose_per_administration == doctest::Approx(500.0));
  CHECK(drug_a.daily_dose.value() == doctest::Approx(1000.0));
  CHECK(drug_a.max_cap_applied);
  CHECK_FALSE(hasCode(drug_a.warnings, "SIN_TOPE_MAXIMO"));

  // Fármaco C: 10 mg/kg/dosis × 50 kg = 500 mg cada 24 h; sin presentaciones IV.
  // El esquema no trae tope: la dosis no se limita y se avisa.
  const RegimenResult& drug_c = option.components[1];
  CHECK(hasCode(drug_c.warnings, "SIN_TOPE_MAXIMO"));
  CHECK(drug_c.dose_per_administration == doctest::Approx(500.0));
  CHECK(drug_c.interval_hours == 24);
  CHECK(drug_c.administrations.empty());
  CHECK(hasCode(drug_c.warnings, "SIN_PRESENTACION_REGISTRADA"));
}

TEST_CASE("motor: dosis fija por rango de peso y dosis única") {
  const KnowledgeBase kb = testing::loadTestKb();

  SUBCASE("lactante de 10 kg: aplica la opción de menos de 27 kg") {
    const TreatmentPlan plan = planFor(kb, testing::infantCase());
    const LineResult& line = lineOf(plan, TreatmentLineKind::First);
    const RegimenResult& result = optionOf(line, 2).components.at(0);

    CHECK(result.unit == DrugUnit::InternationalUnit);
    CHECK(result.dose_per_administration == doctest::Approx(600000.0));
    CHECK_FALSE(result.interval_hours.has_value());  // dosis única
    CHECK_FALSE(result.daily_dose.has_value());
    CHECK(administrationOf(result, "b_vial").rounded_quantity == doctest::Approx(0.5));
    CHECK(hasCode(result.warnings, "DATOS_NO_VERIFICADOS"));  // estado «muestra»

    REQUIRE(line.skipped.size() == 1);
    CHECK(line.skipped[0].number == 3);
    CHECK(line.skipped[0].reason == "requiere peso de al menos 27 kg");
  }

  SUBCASE("adolescente de 50 kg: aplica la opción de 27 kg o más") {
    const TreatmentPlan plan = planFor(kb, testing::adolescentCase());
    const LineResult& line = lineOf(plan, TreatmentLineKind::First);
    const RegimenResult& result = optionOf(line, 3).components.at(0);

    CHECK(result.dose_per_administration == doctest::Approx(1200000.0));
    CHECK(administrationOf(result, "b_vial").rounded_quantity == doctest::Approx(1.0));
    REQUIRE(line.skipped.size() == 1);
    CHECK(line.skipped[0].number == 2);
    CHECK(line.skipped[0].reason == "solo para peso menor de 27 kg");
  }

  SUBCASE("el límite de peso pertenece al rango superior") {
    Patient patient = testing::adolescentCase();
    patient.weight = Kilograms(27.0);
    const TreatmentPlan plan = planFor(kb, patient);
    const LineResult& line = lineOf(plan, TreatmentLineKind::First);
    CHECK(optionOf(line, 3).components.at(0).dose_per_administration ==
          doctest::Approx(1200000.0));
  }
}

TEST_CASE("motor: rango de edad e indicación sin cálculo") {
  const KnowledgeBase kb = testing::loadTestKb();

  const TreatmentPlan infant_plan = planFor(kb, testing::infantCase());
  const LineResult& infant_line = lineOf(infant_plan, TreatmentLineKind::Allergy);
  // Opción 1 (tópica): no se calcula; solo se muestra la indicación.
  const RegimenResult& topical = optionOf(infant_line, 1).components.at(0);
  CHECK_FALSE(topical.calculated);
  CHECK(topical.note == "Aplicar cada 8 horas.");
  CHECK(topical.administrations.empty());
  // Opción 2: exige 2 años; el lactante de 1 año queda excluido.
  REQUIRE(infant_line.skipped.size() == 1);
  CHECK(infant_line.skipped[0].reason == "requiere edad de al menos 2 años");

  const TreatmentPlan adolescent_plan = planFor(kb, testing::adolescentCase());
  const LineResult& adolescent_line = lineOf(adolescent_plan, TreatmentLineKind::Allergy);
  CHECK(adolescent_line.skipped.empty());
  CHECK(optionOf(adolescent_line, 2).components.at(0).dose_per_administration ==
        doctest::Approx(500.0));
}

TEST_CASE("motor: ajuste renal por bandas") {
  const KnowledgeBase kb = testing::loadTestKb();
  const Patient patient = testing::adolescentCase();
  // Sin ajuste, la opción 1 da 500 mg cada 8 h (ver prueba del tope diario).

  const auto firstOption = [&](double gfr) {
    const TreatmentPlan plan = planFor(kb, patient, RenalFunction::entered(Gfr(gfr)));
    return optionOf(lineOf(plan, TreatmentLineKind::First), 1).components.at(0);
  };

  SUBCASE("TFG 60: fuera de las bandas, sin cambio") {
    const RegimenResult result = firstOption(60.0);
    CHECK(result.dose_per_administration == doctest::Approx(500.0));
    CHECK(result.interval_hours == 8);
    CHECK_FALSE(result.renal_adjustment_applied);
  }

  SUBCASE("TFG 40: mitad de dosis, mismo intervalo") {
    const RegimenResult result = firstOption(40.0);
    CHECK(result.dose_per_administration == doctest::Approx(250.0));
    CHECK(result.interval_hours == 8);
    CHECK(result.daily_dose.value() == doctest::Approx(750.0));
    CHECK(result.renal_adjustment_applied);
    CHECK(hasCode(result.warnings, "AJUSTE_RENAL_APLICADO"));
    // La presentación se calcula con la dosis YA ajustada.
    CHECK(administrationOf(result, "a_susp").rounded_quantity == doctest::Approx(5.0));
    // Ajuste verificado: sin advertencia de "no verificado".
    CHECK_FALSE(hasCode(result.warnings, "AJUSTE_RENAL_NO_VERIFICADO"));
  }

  SUBCASE("TFG 20: mitad de dosis e intervalo mínimo de 24 h") {
    const RegimenResult result = firstOption(20.0);
    CHECK(result.dose_per_administration == doctest::Approx(250.0));
    CHECK(result.interval_hours == 24);
    CHECK(result.daily_dose.value() == doctest::Approx(250.0));
  }

  SUBCASE("TFG 5: evitar el medicamento") {
    const RegimenResult result = firstOption(5.0);
    CHECK(result.not_recommended);
    CHECK(hasCode(result.warnings, "EVITAR_POR_FUNCION_RENAL"));
    CHECK(hasBlocking(result.warnings));
    CHECK(result.administrations.empty());  // no se propone cómo administrarlo
  }
}

TEST_CASE("motor: ajuste renal con dosis de reemplazo y tope de la banda") {
  const KnowledgeBase kb = testing::loadTestKb();
  const DoseEngine engine(kb);
  const auto regimen = [&](const Patient& patient, double gfr) {
    const TreatmentPlan plan =
        engine.plan({patient, RenalFunction::entered(Gfr(gfr)), "enfermedad_renal"});
    return optionOf(lineOf(plan, TreatmentLineKind::First), 1).components.at(0);
  };

  // Sin compromiso renal: 90 mg/kg/día × 10 kg ÷ 3 = 300 mg cada 8 h.
  const RegimenResult normal = regimen(testing::infantCase(), 100.0);
  CHECK(normal.dose_per_administration == doctest::Approx(300.0));
  CHECK(normal.interval_hours == 8);

  // TFG 20: la banda reemplaza la dosis por 15 mg/kg × 10 kg = 150 mg, cada 12 h.
  const RegimenResult infant = regimen(testing::infantCase(), 20.0);
  CHECK(infant.dose_per_administration == doctest::Approx(150.0));
  CHECK(infant.interval_hours == 12);
  CHECK(infant.daily_dose.value() == doctest::Approx(300.0));
  CHECK(hasCode(infant.warnings, "AJUSTE_RENAL_APLICADO"));

  // En el adolescente 15 × 50 = 750 mg supera el tope de la banda: 500 mg.
  const RegimenResult adolescent = regimen(testing::adolescentCase(), 20.0);
  CHECK(adolescent.dose_per_administration == doctest::Approx(500.0));
  CHECK(adolescent.interval_hours == 12);

  // El redondeo a la presentación tampoco supera el tope de la banda. Con
  // 34 kg: 15 × 34 = 510 → tope 500 mg; en una suspensión de 150 mg/mL serían
  // 3,33 mL, que redondeados hacia arriba darían 3,5 mL (525 mg).
  // (Caso hallado por la prueba exhaustiva con la población modelada.)
  {
    KnowledgeBase with_liquid = testing::loadTestKb();
    Drug& drug_d = const_cast<Drug&>(with_liquid.drug("farmaco_d"));
    Presentation suspension;
    suspension.id = "d_susp_750";
    suspension.description = "Suspensión 750 mg/5 mL";
    suspension.route = Route::Oral;
    suspension.dispensing_unit = "mL";
    suspension.amount_per_unit = 150.0;
    suspension.rounding_step = 0.5;
    drug_d.presentations.push_back(suspension);
    const DoseEngine capped_engine(with_liquid);
    const Patient heavy{Age::fromYearsAndMonths(7, 0), Kilograms(34.0), Centimeters(120.0)};
    const TreatmentPlan plan =
        capped_engine.plan({heavy, RenalFunction::entered(Gfr(20.0)), "enfermedad_renal"});
    const RegimenResult& result = optionOf(lineOf(plan, TreatmentLineKind::First), 1).components.at(0);
    CHECK(result.dose_per_administration == doctest::Approx(500.0));
    const Administration& a = administrationOf(result, "d_susp_750");
    CHECK(a.rounded_quantity == doctest::Approx(3.0));
    CHECK(a.delivered_amount <= 500.0 + 1e-9);
  }

  // Tope del medicamento: el esquema solo trae tope diario (4000 mg); el tope
  // por toma lo pone el medicamento. 90 × 50 ÷ 3 = 1500 mg → 1000 mg.
  const RegimenResult heavy_normal = regimen(testing::adolescentCase(), 100.0);
  CHECK(heavy_normal.dose_per_administration == doctest::Approx(1000.0));
  CHECK(hasCode(heavy_normal.warnings, "TOPE_DEL_MEDICAMENTO_APLICADO"));
  CHECK_FALSE(hasCode(heavy_normal.warnings, "SIN_TOPE_MAXIMO"));
  CHECK(normal.warnings.size() == regimen(testing::infantCase(), 100.0).warnings.size());
  CHECK_FALSE(hasCode(normal.warnings, "TOPE_DEL_MEDICAMENTO_APLICADO"));

  // TFG 5: la banda no cambia la dosis, pero su nota se muestra como aviso.
  const RegimenResult severe = regimen(testing::infantCase(), 5.0);
  CHECK(severe.dose_per_administration == doctest::Approx(300.0));
  CHECK(severe.interval_hours == 8);
  CHECK(hasCode(severe.warnings, "NOTA_AJUSTE_RENAL"));
  CHECK_FALSE(hasCode(severe.warnings, "AJUSTE_RENAL_APLICADO"));
}

TEST_CASE("motor: medicamento sin dato o sin necesidad de ajuste renal") {
  const KnowledgeBase kb = testing::loadTestKb();
  const TreatmentPlan plan =
      planFor(kb, testing::adolescentCase(), RenalFunction::entered(Gfr(40.0)));
  CHECK_FALSE(hasCode(plan.warnings, "FUNCION_RENAL_NO_EVALUADA"));

  // Fármaco B: la base no tiene el dato → se advierte y NO se ajusta.
  const RegimenResult& no_data =
      optionOf(lineOf(plan, TreatmentLineKind::First), 3).components.at(0);
  CHECK(hasCode(no_data.warnings, "SIN_DATO_AJUSTE_RENAL"));
  CHECK(no_data.dose_per_administration == doctest::Approx(1200000.0));
  CHECK_FALSE(no_data.renal_adjustment_applied);

  // Fármaco C: no requiere ajuste → sin advertencias renales.
  const RegimenResult& not_required =
      optionOf(lineOf(plan, TreatmentLineKind::Second), 1).components.at(1);
  CHECK_FALSE(hasCode(not_required.warnings, "SIN_DATO_AJUSTE_RENAL"));
  CHECK_FALSE(not_required.renal_adjustment_applied);
}

TEST_CASE("motor: el redondeo no sobrepasa el tope por toma") {
  const KnowledgeBase kb = testing::loadTestKb();
  Regimen regimen = kb.disease("enfermedad_x").lines[1].options[0].components[0];
  regimen.max_per_dose = 490.0;

  const RegimenResult result =
      DoseEngine(kb).calculate(testing::adolescentCase(), RenalFunction::assumedNormal(), regimen);
  CHECK(result.dose_per_administration == doctest::Approx(490.0));

  // 490 mg ÷ 50 mg/mL = 9,8 mL. Lo más cercano es 10 mL (500 mg), que supera
  // el tope de 490 mg; se redondea hacia abajo a 9,5 mL (475 mg).
  const Administration& suspension = administrationOf(result, "a_susp");
  CHECK(suspension.rounded_quantity == doctest::Approx(9.5));
  CHECK(suspension.delivered_amount == doctest::Approx(475.0));
  CHECK(suspension.suitable);
}

TEST_CASE("motor: datos del paciente que bloquean el cálculo") {
  const KnowledgeBase kb = testing::loadTestKb();

  const Patient neonate{Age::fromMonths(0), Kilograms(3.5), std::nullopt};
  const TreatmentPlan neonate_plan = planFor(kb, neonate);
  CHECK(neonate_plan.blocked);
  CHECK(neonate_plan.lines.empty());
  CHECK(hasCode(neonate_plan.warnings, "EDAD_NEONATO"));

  const TreatmentPlan bad_gfr_plan =
      planFor(kb, testing::infantCase(), RenalFunction::entered(Gfr(-5.0)));
  CHECK(bad_gfr_plan.blocked);
  CHECK(hasCode(bad_gfr_plan.warnings, "TFG_INVALIDA"));
}

TEST_CASE("motor: enfermedad sin indicación de antibiótico") {
  const KnowledgeBase kb = testing::loadTestKb();
  const TreatmentPlan plan = DoseEngine(kb).plan(
      {testing::infantCase(), RenalFunction::assumedNormal(), "enfermedad_viral"});
  CHECK_FALSE(plan.blocked);
  CHECK_FALSE(plan.antibiotic_indicated);
  CHECK(plan.message == "No requiere antibiótico.");
  CHECK(plan.lines.empty());
}

TEST_CASE("motor: enfermedad inexistente") {
  const KnowledgeBase kb = testing::loadTestKb();
  CHECK_THROWS_AS((void)DoseEngine(kb).plan({testing::infantCase(),
                                             RenalFunction::assumedNormal(), "no_existe"}),
                  NotFoundError);
}

TEST_CASE("motor: es determinista") {
  const KnowledgeBase kb = testing::loadTestKb();
  const RenalFunction renal = RenalFunction::entered(Gfr(40.0));
  const TreatmentPlan first = planFor(kb, testing::infantCase(), renal);
  const TreatmentPlan second = planFor(kb, testing::infantCase(), renal);

  REQUIRE(first.lines.size() == second.lines.size());
  for (std::size_t i = 0; i < first.lines.size(); ++i) {
    REQUIRE(first.lines[i].options.size() == second.lines[i].options.size());
    for (std::size_t j = 0; j < first.lines[i].options.size(); ++j) {
      const RegimenResult& a = first.lines[i].options[j].components.at(0);
      const RegimenResult& b = second.lines[i].options[j].components.at(0);
      CHECK(a.dose_per_administration == b.dose_per_administration);  // igualdad exacta
      CHECK(a.interval_hours == b.interval_hours);
    }
  }
}
