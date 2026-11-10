/// @file units.hpp
/// @brief Magnitudes físicas con tipos fuertes y conversiones de unidades.
///
/// El error clásico al calcular dosis es mezclar unidades (kg con lb, mg con
/// mL). Aquí cada magnitud tiene su propio tipo: el compilador rechaza, por
/// ejemplo, pasar una talla donde se espera un peso. Para obtener el número
/// "crudo" hay que pedirlo explícitamente con `.value()`.
///
/// Las cantidades de principio activo (mg o UI) no usan estos tipos porque su
/// unidad depende del medicamento y se conoce solo al leer los datos; ver
/// `DrugUnit` en knowledge_base.hpp.
#pragma once

namespace calc {

/// Magnitud escalar etiquetada. `Tag` solo sirve para distinguir tipos.
template <typename Tag>
class Quantity {
 public:
  constexpr Quantity() noexcept = default;
  constexpr explicit Quantity(double value) noexcept : value_(value) {}

  /// Valor numérico en la unidad propia del tipo.
  [[nodiscard]] constexpr double value() const noexcept { return value_; }

  friend constexpr Quantity operator+(Quantity a, Quantity b) noexcept {
    return Quantity(a.value_ + b.value_);
  }
  friend constexpr Quantity operator-(Quantity a, Quantity b) noexcept {
    return Quantity(a.value_ - b.value_);
  }
  friend constexpr Quantity operator*(Quantity a, double k) noexcept {
    return Quantity(a.value_ * k);
  }
  friend constexpr Quantity operator*(double k, Quantity a) noexcept {
    return Quantity(a.value_ * k);
  }
  friend constexpr Quantity operator/(Quantity a, double k) noexcept {
    return Quantity(a.value_ / k);
  }
  /// Cociente de dos magnitudes iguales: número sin unidad.
  friend constexpr double operator/(Quantity a, Quantity b) noexcept {
    return a.value_ / b.value_;
  }

  friend constexpr bool operator==(Quantity a, Quantity b) noexcept { return a.value_ == b.value_; }
  friend constexpr bool operator!=(Quantity a, Quantity b) noexcept { return a.value_ != b.value_; }
  friend constexpr bool operator<(Quantity a, Quantity b) noexcept { return a.value_ < b.value_; }
  friend constexpr bool operator<=(Quantity a, Quantity b) noexcept { return a.value_ <= b.value_; }
  friend constexpr bool operator>(Quantity a, Quantity b) noexcept { return a.value_ > b.value_; }
  friend constexpr bool operator>=(Quantity a, Quantity b) noexcept { return a.value_ >= b.value_; }

 private:
  double value_ = 0.0;
};

namespace tags {
struct Kilograms;
struct Centimeters;
struct Milliliters;
struct MgPerDl;
struct Gfr;
}  // namespace tags

using Kilograms = Quantity<tags::Kilograms>;      ///< Peso corporal (kg).
using Centimeters = Quantity<tags::Centimeters>;  ///< Talla (cm).
using Milliliters = Quantity<tags::Milliliters>;  ///< Volumen (mL).
using MgPerDl = Quantity<tags::MgPerDl>;          ///< Creatinina sérica (mg/dL).
using Gfr = Quantity<tags::Gfr>;                  ///< TFG (mL/min/1,73 m²).

// --- Conversiones ("regla de tres" programada una sola vez) ------------------

inline constexpr double kKilogramsPerPound = 0.45359237;  ///< Definición exacta.
inline constexpr double kMicromolPerLPerMgPerDlCreatinine = 88.4;

[[nodiscard]] constexpr Kilograms kilogramsFromPounds(double pounds) noexcept {
  return Kilograms(pounds * kKilogramsPerPound);
}
[[nodiscard]] constexpr Kilograms kilogramsFromGrams(double grams) noexcept {
  return Kilograms(grams / 1000.0);
}
[[nodiscard]] constexpr Centimeters centimetersFromMeters(double meters) noexcept {
  return Centimeters(meters * 100.0);
}
/// Creatinina: µmol/L → mg/dL.
[[nodiscard]] constexpr MgPerDl mgPerDlFromMicromolPerL(double micromol_per_l) noexcept {
  return MgPerDl(micromol_per_l / kMicromolPerLPerMgPerDlCreatinine);
}
[[nodiscard]] constexpr double milligramsFromGrams(double grams) noexcept {
  return grams * 1000.0;
}
[[nodiscard]] constexpr double milligramsFromMicrograms(double micrograms) noexcept {
  return micrograms / 1000.0;
}

}  // namespace calc
