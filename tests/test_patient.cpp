#include <doctest/doctest.h>

#include <stdexcept>

#include "calc/patient.hpp"
#include "support/test_kb.hpp"

using namespace calc;

TEST_CASE("edad: construcción y descripción") {
  CHECK(Age::fromYearsAndMonths(1, 0).totalMonths() == 12);
  CHECK(Age::fromYearsAndMonths(2, 3).totalMonths() == 27);
  CHECK(Age::fromMonths(18).years() == doctest::Approx(1.5));

  CHECK(Age::fromMonths(1).describe() == "1 mes");
  CHECK(Age::fromMonths(8).describe() == "8 meses");
  CHECK(Age::fromMonths(12).describe() == "1 año");
  CHECK(Age::fromMonths(156).describe() == "13 años");
  CHECK(Age::fromMonths(27).describe() == "2 años 3 meses");

  CHECK_THROWS_AS((void)Age::fromMonths(-1), std::invalid_argument);
  CHECK_THROWS_AS((void)Age::fromYearsAndMonths(1, 12), std::invalid_argument);
}

TEST_CASE("paciente: los dos casos tipo no generan advertencias") {
  CHECK(validatePatient(testing::infantCase()).empty());
  CHECK(validatePatient(testing::adolescentCase()).empty());
}

TEST_CASE("paciente: alcance por edad") {
  const Patient neonate{Age::fromMonths(0), Kilograms(3.5), std::nullopt};
  const auto neonate_warnings = validatePatient(neonate);
  CHECK(hasBlocking(neonate_warnings));
  CHECK(hasCode(neonate_warnings, "EDAD_NEONATO"));

  const Patient adult{Age::fromYearsAndMonths(18, 0), Kilograms(70.0), std::nullopt};
  CHECK(hasCode(validatePatient(adult), "EDAD_FUERA_DE_ALCANCE"));

  const Patient almost_adult{Age::fromYearsAndMonths(17, 11), Kilograms(60.0), std::nullopt};
  CHECK_FALSE(hasBlocking(validatePatient(almost_adult)));
}

TEST_CASE("paciente: peso y talla inválidos bloquean") {
  Patient patient = testing::infantCase();
  patient.weight = Kilograms(0.0);
  CHECK(hasCode(validatePatient(patient), "PESO_INVALIDO"));

  patient.weight = Kilograms(250.0);
  CHECK(hasCode(validatePatient(patient), "PESO_INVALIDO"));

  patient = testing::infantCase();
  patient.height = Centimeters(0.75);  // digitada en metros
  const auto warnings = validatePatient(patient);
  CHECK(hasCode(warnings, "TALLA_INVALIDA"));
  CHECK(hasBlocking(warnings));
}

TEST_CASE("paciente: un peso poco plausible advierte pero no bloquea") {
  // 30 kg en un lactante de 1 año: probable error de digitación.
  Patient patient = testing::infantCase();
  patient.weight = Kilograms(30.0);
  const auto warnings = validatePatient(patient);
  CHECK(hasCode(warnings, "PESO_POCO_PLAUSIBLE"));
  CHECK_FALSE(hasBlocking(warnings));
}

TEST_CASE("paciente: peso esperado para la edad") {
  CHECK(expectedWeightForAge(Age::fromMonths(6)).value() == doctest::Approx(7.0));
  CHECK(expectedWeightForAge(Age::fromMonths(12)).value() == doctest::Approx(10.0));
  CHECK(expectedWeightForAge(Age::fromYearsAndMonths(13, 0)).value() == doctest::Approx(46.0));
}
