/// @file patient.hpp
/// @brief Datos del paciente y su validación.
///
/// Por diseño NO se almacena ningún dato que identifique al paciente (nombre,
/// documento, historia clínica): solo las variables que entran al cálculo.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "calc/units.hpp"
#include "calc/warnings.hpp"

namespace calc {

/// Edad del paciente con resolución de meses.
class Age {
 public:
  /// @throws std::invalid_argument si `months` es negativo.
  [[nodiscard]] static Age fromMonths(int months);
  /// @throws std::invalid_argument si algún valor es negativo o `months` > 11.
  [[nodiscard]] static Age fromYearsAndMonths(int years, int months);

  [[nodiscard]] int totalMonths() const noexcept { return months_; }
  [[nodiscard]] double years() const noexcept { return months_ / 12.0; }

  /// Texto legible: "8 meses", "1 año", "2 años 3 meses".
  [[nodiscard]] std::string describe() const;

 private:
  explicit Age(int months) noexcept : months_(months) {}
  int months_;
};

/// Sexo del paciente. Solo lo usan las fórmulas de función renal que tienen
/// constantes distintas por sexo (p. ej. CKiD U25); las dosis no dependen de él.
enum class Sex { Male, Female };

/// Texto visible: "masculino" o "femenino".
[[nodiscard]] std::string_view label(Sex sex) noexcept;

/// Variables del paciente que entran al cálculo.
struct Patient {
  Age age;
  Kilograms weight;
  std::optional<Centimeters> height = std::nullopt;  ///< Solo para estimar la TFG.
  std::optional<Sex> sex = std::nullopt;             ///< Solo para estimar la TFG.
};

/// Límites de alcance y de plausibilidad de los datos de entrada.
namespace limits {
/// Los neonatos (< 1 mes) quedan fuera del alcance: su dosificación depende de
/// la edad gestacional y posnatal y no cabe en el modelo mg/kg de esta versión.
inline constexpr int kMinAgeMonths = 1;
/// Alcance pediátrico: menores de 18 años.
inline constexpr int kMaxAgeMonthsExclusive = 18 * 12;
inline constexpr double kMaxWeightKg = 200.0;
inline constexpr double kMinHeightCm = 30.0;
inline constexpr double kMaxHeightCm = 220.0;
/// Un peso por fuera de [bajo, alto] × el peso esperado para la edad genera
/// una advertencia (no bloquea): suele indicar un error de digitación o de
/// unidad (lb en lugar de kg).
inline constexpr double kLowWeightRatio = 0.5;
inline constexpr double kHighWeightRatio = 2.5;
}  // namespace limits

/// Peso esperado para la edad con las fórmulas de estimación rápida de uso
/// habitual en urgencias pediátricas (< 1 año: 0,5·meses + 4; 1–5 años:
/// 2·años + 8; ≥ 6 años: 3·años + 7).
///
/// Es una HEURÍSTICA para detectar errores de digitación, no una referencia de
/// crecimiento. Punto de mejora: reemplazar por las tablas de la OMS.
[[nodiscard]] Kilograms expectedWeightForAge(Age age) noexcept;

/// Valida alcance y plausibilidad. Devuelve advertencias; si alguna es
/// bloqueante, no debe calcularse ninguna dosis.
///
/// Códigos: EDAD_NEONATO, EDAD_FUERA_DE_ALCANCE, PESO_INVALIDO,
/// TALLA_INVALIDA, PESO_POCO_PLAUSIBLE.
[[nodiscard]] std::vector<Warning> validatePatient(const Patient& patient);

}  // namespace calc
