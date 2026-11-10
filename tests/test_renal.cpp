#include <doctest/doctest.h>

#include <cmath>

#include "calc/renal.hpp"
#include "support/test_kb.hpp"

using namespace calc;

TEST_CASE("renal: Schwartz de cabecera con los casos tipo") {
  const BedsideSchwartzEstimator estimator;

  // 0,413 × 75 / 0,4 = 77,4375
  const GfrEstimate infant = estimator.estimate(testing::infantCase(), MgPerDl(0.4));
  REQUIRE(infant.gfr.has_value());
  CHECK(infant.gfr->value() == doctest::Approx(77.4375));
  CHECK(infant.warnings.empty());
  CHECK(infant.formula == "0,413 × 75 cm ÷ 0,4 mg/dL = 77,4 mL/min/1,73 m²");

  // 0,413 × 162 / 0,7 = 95,58
  const GfrEstimate adolescent = estimator.estimate(testing::adolescentCase(), MgPerDl(0.7));
  REQUIRE(adolescent.gfr.has_value());
  CHECK(adolescent.gfr->value() == doctest::Approx(95.58));
}

TEST_CASE("renal: Schwartz exige talla y creatinina válidas") {
  const BedsideSchwartzEstimator estimator;

  Patient without_height = testing::infantCase();
  without_height.height.reset();
  const GfrEstimate no_height = estimator.estimate(without_height, MgPerDl(0.4));
  CHECK_FALSE(no_height.gfr.has_value());
  CHECK(hasCode(no_height.warnings, "TALLA_REQUERIDA"));

  const GfrEstimate no_creatinine = estimator.estimate(testing::infantCase(), MgPerDl(0.0));
  CHECK_FALSE(no_creatinine.gfr.has_value());
  CHECK(hasCode(no_creatinine.warnings, "CREATININA_INVALIDA"));

  // Creatinina digitada en µmol/L (35) en lugar de mg/dL.
  const GfrEstimate wrong_unit = estimator.estimate(testing::infantCase(), MgPerDl(35.0));
  CHECK(hasCode(wrong_unit.warnings, "CREATININA_INVALIDA"));
}

TEST_CASE("renal: Schwartz advierte fuera del rango de edad en que se derivó") {
  const BedsideSchwartzEstimator estimator;
  const Patient infant_6m{Age::fromMonths(6), Kilograms(7.0), Centimeters(66.0)};
  const GfrEstimate estimate = estimator.estimate(infant_6m, MgPerDl(0.3));
  CHECK(estimate.gfr.has_value());  // se calcula, pero se advierte
  CHECK(hasCode(estimate.warnings, "SCHWARTZ_FUERA_DE_RANGO_DE_EDAD"));
}

// --- CKiD U25 ----------------------------------------------------------------
//
// Valores esperados calculados aparte con la tabla de Pierce et al. 2021:
//   k = base × crecimiento^(edad − 12);  TFG = k × talla (m) ÷ creatinina.

TEST_CASE("renal: CKiD U25 con los casos tipo, por sexo") {
  const CkidU25Estimator estimator;

  Patient infant = testing::infantCase();  // 1 año, 75 cm
  infant.sex = Sex::Male;
  // k = 39,0 × 1,008^(−11) = 35,7272;  35,7272 × 0,75 ÷ 0,4 = 66,988
  const GfrEstimate boy = estimator.estimate(infant, MgPerDl(0.4));
  REQUIRE(boy.gfr.has_value());
  CHECK(boy.gfr->value() == doctest::Approx(66.9885).epsilon(1e-5));
  CHECK(boy.warnings.empty());
  CHECK(boy.formula ==
        "k (masculino, 1 año) = 39 × 1,008^(1 − 12) = 35,73; "
        "TFG = 35,73 × 0,75 m ÷ 0,4 mg/dL = 67 mL/min/1,73 m²");

  infant.sex = Sex::Female;
  // k = 36,1 × 1,008^(−11) = 33,0705;  33,0705 × 0,75 ÷ 0,4 = 62,007
  const GfrEstimate girl = estimator.estimate(infant, MgPerDl(0.4));
  REQUIRE(girl.gfr.has_value());
  CHECK(girl.gfr->value() == doctest::Approx(62.0073).epsilon(1e-5));

  Patient adolescent = testing::adolescentCase();  // 13 años, 162 cm
  adolescent.sex = Sex::Male;
  // k = 39,0 × 1,045^1 = 40,755;  40,755 × 1,62 ÷ 0,7 = 94,319
  CHECK(estimator.estimate(adolescent, MgPerDl(0.7)).gfr->value() ==
        doctest::Approx(94.3187).epsilon(1e-5));
  adolescent.sex = Sex::Female;
  // k = 36,1 × 1,023^1 = 36,9303;  36,9303 × 1,62 ÷ 0,7 = 85,467
  CHECK(estimator.estimate(adolescent, MgPerDl(0.7)).gfr->value() ==
        doctest::Approx(85.4673).epsilon(1e-5));
}

TEST_CASE("renal: la constante k de CKiD U25 por tramos de edad") {
  // A los 12 años el exponente es 0: k vale la base, en ambos tramos.
  CHECK(CkidU25Estimator::coefficient(Age::fromYearsAndMonths(12, 0), Sex::Male) ==
        doctest::Approx(39.0));
  CHECK(CkidU25Estimator::coefficient(Age::fromYearsAndMonths(12, 0), Sex::Female) ==
        doctest::Approx(36.1));

  // La edad entra con decimales: 6 años 6 meses = 6,5 años.
  CHECK(CkidU25Estimator::coefficient(Age::fromYearsAndMonths(6, 6), Sex::Male) ==
        doctest::Approx(39.0 * std::pow(1.008, -5.5)));

  // Desde los 18 años k es fijo.
  CHECK(CkidU25Estimator::coefficient(Age::fromYearsAndMonths(18, 0), Sex::Male) == 50.8);
  CHECK(CkidU25Estimator::coefficient(Age::fromYearsAndMonths(20, 0), Sex::Female) == 41.4);

  // Coherencia de la tabla: los tramos empalman a los 18 años (un mes antes,
  // k ya está a menos de 1 % del valor fijo). Detecta una constante mal copiada.
  const Age almost_18 = Age::fromYearsAndMonths(17, 11);
  CHECK(CkidU25Estimator::coefficient(almost_18, Sex::Male) == doctest::Approx(50.8).epsilon(0.01));
  CHECK(CkidU25Estimator::coefficient(almost_18, Sex::Female) ==
        doctest::Approx(41.4).epsilon(0.01));
}

TEST_CASE("renal: CKiD U25 exige talla, sexo y creatinina válidas") {
  const CkidU25Estimator estimator;

  // El caso tipo no trae sexo.
  const GfrEstimate no_sex = estimator.estimate(testing::infantCase(), MgPerDl(0.4));
  CHECK_FALSE(no_sex.gfr.has_value());
  CHECK(hasCode(no_sex.warnings, "SEXO_REQUERIDO"));

  Patient patient = testing::infantCase();
  patient.sex = Sex::Female;
  patient.height.reset();
  const GfrEstimate no_height = estimator.estimate(patient, MgPerDl(0.4));
  CHECK_FALSE(no_height.gfr.has_value());
  CHECK(hasCode(no_height.warnings, "TALLA_REQUERIDA"));

  patient.height = Centimeters(75.0);
  CHECK(hasCode(estimator.estimate(patient, MgPerDl(0.0)).warnings, "CREATININA_INVALIDA"));
}

TEST_CASE("renal: CKiD U25 advierte en menores de 1 año") {
  const CkidU25Estimator estimator;
  const Patient infant_6m{Age::fromMonths(6), Kilograms(7.0), Centimeters(66.0), Sex::Male};
  const GfrEstimate estimate = estimator.estimate(infant_6m, MgPerDl(0.3));
  CHECK(estimate.gfr.has_value());  // se calcula, pero se advierte
  CHECK(hasCode(estimate.warnings, "CKID_U25_FUERA_DE_RANGO_DE_EDAD"));
}

TEST_CASE("renal: catálogo de fórmulas") {
  CHECK(kDefaultGfrFormula == GfrFormula::CkidU25);
  for (const GfrFormula formula : kGfrFormulas) {
    // Cada fórmula tiene identificador, se recupera por él y crea su estimador.
    CHECK_FALSE(id(formula).empty());
    CHECK(gfrFormulaFromId(id(formula)) == formula);
    REQUIRE(makeGfrEstimator(formula) != nullptr);
    CHECK_FALSE(makeGfrEstimator(formula)->name().empty());
  }
  CHECK(makeGfrEstimator(GfrFormula::CkidU25)->name() == "CKiD U25");
  CHECK(makeGfrEstimator(GfrFormula::BedsideSchwartz)->name() == "Schwartz de cabecera");
  CHECK_FALSE(gfrFormulaFromId("cockcroft").has_value());
}

TEST_CASE("renal: validación de la TFG y constructores") {
  CHECK(validateGfr(Gfr(40.0)).empty());
  CHECK(hasCode(validateGfr(Gfr(0.0)), "TFG_INVALIDA"));
  CHECK(hasCode(validateGfr(Gfr(400.0)), "TFG_INVALIDA"));

  CHECK_FALSE(RenalFunction::assumedNormal().gfr.has_value());
  CHECK(RenalFunction::entered(Gfr(40.0)).source == RenalSource::EnteredDirectly);
  const RenalFunction estimated = RenalFunction::estimated(Gfr(77.4), "Schwartz de cabecera");
  CHECK(estimated.source == RenalSource::Estimated);
  CHECK(estimated.method == "Schwartz de cabecera");
}
