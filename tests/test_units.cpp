#include <doctest/doctest.h>

#include "calc/units.hpp"

using namespace calc;

TEST_CASE("unidades: aritmética entre magnitudes del mismo tipo") {
  const Kilograms a(10.0);
  const Kilograms b(2.5);
  CHECK((a + b).value() == doctest::Approx(12.5));
  CHECK((a - b).value() == doctest::Approx(7.5));
  CHECK((a * 2.0).value() == doctest::Approx(20.0));
  CHECK((2.0 * a).value() == doctest::Approx(20.0));
  CHECK((a / 4.0).value() == doctest::Approx(2.5));
  CHECK(a / b == doctest::Approx(4.0));  // cociente sin unidad
  CHECK(b < a);
  CHECK(a == Kilograms(10.0));
}

TEST_CASE("unidades: conversiones") {
  CHECK(kilogramsFromPounds(22.0).value() == doctest::Approx(9.979).epsilon(0.001));
  CHECK(kilogramsFromGrams(3500.0).value() == doctest::Approx(3.5));
  CHECK(centimetersFromMeters(1.62).value() == doctest::Approx(162.0));
  CHECK(mgPerDlFromMicromolPerL(88.4).value() == doctest::Approx(1.0));
  CHECK(milligramsFromGrams(1.5) == doctest::Approx(1500.0));
  CHECK(milligramsFromMicrograms(250.0) == doctest::Approx(0.25));
}
