#include "calc/patient.hpp"

#include <stdexcept>

#include "calc/text.hpp"

namespace calc {

// --- Age ---------------------------------------------------------------------

Age Age::fromMonths(int months) {
  if (months < 0) {
    throw std::invalid_argument("La edad no puede ser negativa");
  }
  return Age(months);
}

Age Age::fromYearsAndMonths(int years, int months) {
  if (years < 0 || months < 0 || months > 11) {
    throw std::invalid_argument("Edad inválida: se esperan años >= 0 y meses entre 0 y 11");
  }
  return Age(years * 12 + months);
}

std::string Age::describe() const {
  const int years = months_ / 12;
  const int months = months_ % 12;
  const auto plural = [](int n, const char* singular, const char* many) {
    return std::to_string(n) + " " + (n == 1 ? singular : many);
  };
  if (years == 0) {
    return plural(months, "mes", "meses");
  }
  if (months == 0) {
    return plural(years, "año", "años");
  }
  return plural(years, "año", "años") + " " + plural(months, "mes", "meses");
}

// --- Validación --------------------------------------------------------------

Kilograms expectedWeightForAge(Age age) noexcept {
  const int months = age.totalMonths();
  const double years = age.years();
  if (months < 12) {
    return Kilograms(0.5 * months + 4.0);
  }
  if (years < 6.0) {
    return Kilograms(2.0 * years + 8.0);
  }
  return Kilograms(3.0 * years + 7.0);
}

std::vector<Warning> validatePatient(const Patient& patient) {
  std::vector<Warning> warnings;
  const int months = patient.age.totalMonths();
  const double kg = patient.weight.value();

  // Alcance por edad.
  if (months < limits::kMinAgeMonths) {
    warnings.push_back({Severity::Blocking, "EDAD_NEONATO",
                        "Los menores de 1 mes están fuera del alcance de la calculadora: la "
                        "dosificación neonatal depende de la edad gestacional y posnatal."});
  } else if (months >= limits::kMaxAgeMonthsExclusive) {
    warnings.push_back({Severity::Blocking, "EDAD_FUERA_DE_ALCANCE",
                        "La calculadora es pediátrica: solo admite menores de 18 años."});
  }

  // Peso: primero que sea un número utilizable, luego que sea plausible.
  if (!(kg > 0.0) || kg > limits::kMaxWeightKg) {
    warnings.push_back({Severity::Blocking, "PESO_INVALIDO",
                        "El peso debe ser mayor que 0 y no superar " +
                            formatNumber(limits::kMaxWeightKg) + " kg."});
  } else if (!hasBlocking(warnings)) {
    const double expected = expectedWeightForAge(patient.age).value();
    if (kg < limits::kLowWeightRatio * expected || kg > limits::kHighWeightRatio * expected) {
      warnings.push_back({Severity::Caution, "PESO_POCO_PLAUSIBLE",
                          "El peso (" + formatNumber(kg) + " kg) es poco habitual para la edad (" +
                              patient.age.describe() + "; esperado cerca de " +
                              formatNumber(expected, 1) +
                              " kg). Verifique el valor y que esté en kilogramos."});
    }
  }

  if (patient.height) {
    const double cm = patient.height->value();
    if (cm < limits::kMinHeightCm || cm > limits::kMaxHeightCm) {
      warnings.push_back({Severity::Blocking, "TALLA_INVALIDA",
                          "La talla debe estar entre " + formatNumber(limits::kMinHeightCm) +
                              " y " + formatNumber(limits::kMaxHeightCm) +
                              " cm. Verifique que esté en centímetros."});
    }
  }

  return warnings;
}

}  // namespace calc
