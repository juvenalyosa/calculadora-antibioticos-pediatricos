#include <doctest/doctest.h>

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
