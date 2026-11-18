#include <doctest/doctest.h>

#include "calc/text.hpp"

using namespace calc;

TEST_CASE("texto: formato de números") {
  CHECK(formatNumber(450.0) == "450");
  CHECK(formatNumber(4.5) == "4,5");
  CHECK(formatNumber(3.75) == "3,75");
  CHECK(formatNumber(166.6667) == "166,67");
  CHECK(formatNumber(77.4375, 1) == "77,4");
  CHECK(formatNumber(0.0) == "0");
  CHECK(formatNumber(-0.001) == "0");
  CHECK(formatNumber(-2.5) == "-2,5");

  // Miles: sin separador hasta cuatro cifras, con espacio desde cinco.
  CHECK(formatNumber(4000.0) == "4000");
  CHECK(formatNumber(10000.0) == "10 000");
  CHECK(formatNumber(600000.0) == "600 000");
  CHECK(formatNumber(1200000.0) == "1 200 000");
}

TEST_CASE("texto: cantidades, intervalos y administración") {
  CHECK(formatAmount(450.0, DrugUnit::Milligram) == "450 mg");
  CHECK(formatAmount(600000.0, DrugUnit::InternationalUnit) == "600 000 UI");

  CHECK(describeInterval(12) == "cada 12 h");
  CHECK(describeInterval(std::nullopt) == "dosis única");

  Administration administration;
  administration.dispensing_unit = "mL";
  administration.rounded_quantity = 9.0;
  CHECK(describeAdministration(administration) == "9 mL");

  administration.dispensing_unit = "tableta";
  administration.rounded_quantity = 0.5;
  CHECK(describeAdministration(administration) == "0,5 tableta");
  administration.rounded_quantity = 2.0;
  CHECK(describeAdministration(administration) == "2 tabletas");

  administration.dispensing_unit = "vial";
  CHECK(describeAdministration(administration) == "2 viales");

  // La cantidad exacta del desglose usa el mismo plural.
  CHECK(describeQuantity(1.5, "cápsula") == "1,5 cápsulas");
  CHECK(describeQuantity(0.75, "tableta") == "0,75 tableta");
  CHECK(describeQuantity(26.67, "mL") == "26,67 mL");
}

TEST_CASE("texto: dosis y duración") {
  RegimenResult result;
  result.calculated = true;
  result.unit = DrugUnit::Milligram;
  result.dose_per_administration = 450.0;
  result.interval_hours = 12;
  result.daily_dose = 900.0;
  CHECK(describeDose(result) == "450 mg cada 12 h (900 mg/día)");

  result.interval_hours.reset();
  result.daily_dose.reset();
  CHECK(describeDose(result) == "450 mg dosis única");

  CHECK(describeDuration(result).empty());
  result.duration_min_days = 5;
  result.duration_max_days = 10;
  CHECK(describeDuration(result) == "5 a 10 días");
  result.duration_min_days = 10;
  CHECK(describeDuration(result) == "10 días");
}

TEST_CASE("texto: función renal") {
  CHECK(describeRenal(RenalFunction::assumedNormal()) == "no evaluada (se asume normal)");
  CHECK(describeRenal(RenalFunction::entered(Gfr(40.0))) == "TFG 40 mL/min/1,73 m² (digitada)");
  CHECK(describeRenal(RenalFunction::estimated(Gfr(77.44), "Schwartz de cabecera")) ==
        "TFG 77,4 mL/min/1,73 m² (Schwartz de cabecera)");
}
