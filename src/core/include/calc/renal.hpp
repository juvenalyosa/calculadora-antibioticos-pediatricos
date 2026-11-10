/// @file renal.hpp
/// @brief Función renal del paciente y estimación de la TFG.
///
/// La estimación está detrás de la interfaz `GfrEstimator` para poder añadir
/// otras fórmulas (p. ej. con cistatina C) sin tocar el motor de dosis. Hoy
/// hay dos: CKiD U25 (por defecto) y Schwartz de cabecera.
#pragma once

#include <array>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "calc/patient.hpp"
#include "calc/units.hpp"
#include "calc/warnings.hpp"

namespace calc {

/// De dónde salió el valor de TFG que usa el cálculo.
enum class RenalSource {
  AssumedNormal,    ///< El usuario no evaluó la función renal.
  EnteredDirectly,  ///< El usuario digitó la TFG.
  Estimated         ///< Se estimó con una fórmula (ver `method`).
};

/// Función renal que recibe el motor de dosis.
struct RenalFunction {
  RenalSource source = RenalSource::AssumedNormal;
  std::optional<Gfr> gfr;  ///< Vacío cuando se asume normal.
  std::string method;      ///< Nombre de la fórmula, si se estimó.

  [[nodiscard]] static RenalFunction assumedNormal();
  [[nodiscard]] static RenalFunction entered(Gfr gfr);
  [[nodiscard]] static RenalFunction estimated(Gfr gfr, std::string method);
};

namespace limits {
inline constexpr double kMaxPlausibleGfr = 250.0;
inline constexpr double kMaxPlausibleCreatinineMgDl = 20.0;
/// Por debajo de este valor se avisa cuando la base no tiene información de
/// ajuste renal para un medicamento (límite inferior de TFG normal, KDIGO G1).
inline constexpr double kReducedGfrThreshold = 90.0;
}  // namespace limits

/// Valida una TFG digitada o estimada. Código: TFG_INVALIDA.
[[nodiscard]] std::vector<Warning> validateGfr(Gfr gfr);

/// Resultado de una estimación de TFG.
struct GfrEstimate {
  std::optional<Gfr> gfr;         ///< Vacío si no se pudo estimar.
  std::string formula;            ///< Cálculo desglosado, para mostrar al usuario.
  std::vector<Warning> warnings;  ///< Bloqueantes si no se pudo estimar.
};

/// Interfaz de una fórmula de estimación de la TFG.
class GfrEstimator {
 public:
  virtual ~GfrEstimator() = default;
  /// Nombre visible de la fórmula.
  [[nodiscard]] virtual std::string name() const = 0;
  [[nodiscard]] virtual GfrEstimate estimate(const Patient& patient,
                                             MgPerDl serum_creatinine) const = 0;
};

/// Fórmula de Schwartz "de cabecera" (Schwartz et al., J Am Soc Nephrol 2009):
///
///     TFG = 0,413 × talla (cm) / creatinina sérica (mg/dL)
///
/// Requiere creatinina medida por método enzimático (trazable a IDMS). Se
/// derivó en niños de 1 a 16 años; fuera de ese rango se calcula pero se
/// advierte. Códigos: TALLA_REQUERIDA, CREATININA_INVALIDA,
/// SCHWARTZ_FUERA_DE_RANGO_DE_EDAD.
class BedsideSchwartzEstimator final : public GfrEstimator {
 public:
  static constexpr double kCoefficient = 0.413;
  static constexpr int kValidatedMinAgeMonths = 12;
  static constexpr int kValidatedMaxAgeMonthsExclusive = 17 * 12;

  [[nodiscard]] std::string name() const override;
  [[nodiscard]] GfrEstimate estimate(const Patient& patient,
                                     MgPerDl serum_creatinine) const override;
};

/// Ecuación CKiD U25 con creatinina (Pierce et al., Kidney Int 2021;99:948-956):
///
///     TFG = k × talla (m) / creatinina sérica (mg/dL)
///
/// A diferencia de Schwartz de cabecera (k fijo de 41,3), aquí k depende de la
/// edad en años (con decimales) y del sexo:
///
///     Edad            Masculino                   Femenino
///     < 12 años       39,0 × 1,008^(edad − 12)    36,1 × 1,008^(edad − 12)
///     12 a < 18 años  39,0 × 1,045^(edad − 12)    36,1 × 1,023^(edad − 12)
///     18 a 25 años    50,8                        41,4
///
/// Se derivó en personas de 1 a 25 años con enfermedad renal crónica y
/// creatinina enzimática. En menores de 1 año se calcula pero se advierte.
/// Códigos: TALLA_REQUERIDA, SEXO_REQUERIDO, CREATININA_INVALIDA,
/// CKID_U25_FUERA_DE_RANGO_DE_EDAD.
class CkidU25Estimator final : public GfrEstimator {
 public:
  static constexpr int kValidatedMinAgeMonths = 12;
  static constexpr int kValidatedMaxAgeMonthsExclusive = 26 * 12;

  /// Constante k para una edad y un sexo.
  [[nodiscard]] static double coefficient(Age age, Sex sex) noexcept;

  [[nodiscard]] std::string name() const override;
  [[nodiscard]] GfrEstimate estimate(const Patient& patient,
                                     MgPerDl serum_creatinine) const override;
};

// --- Catálogo de fórmulas ----------------------------------------------------

/// Fórmulas disponibles para estimar la TFG a partir de la creatinina.
/// Al añadir una: agregarla aquí, en `kGfrFormulas` y en las tres funciones.
enum class GfrFormula { CkidU25, BedsideSchwartz };

/// Todas las fórmulas, en el orden en que las ofrecen las interfaces.
inline constexpr std::array<GfrFormula, 2> kGfrFormulas = {GfrFormula::CkidU25,
                                                           GfrFormula::BedsideSchwartz};

/// Fórmula que se usa si el usuario no elige otra. Decisión por confirmar con
/// el equipo clínico (ver docs/arquitectura.md).
inline constexpr GfrFormula kDefaultGfrFormula = GfrFormula::CkidU25;

/// Identificador estable para la línea de órdenes: "ckid-u25", "schwartz".
[[nodiscard]] std::string_view id(GfrFormula formula) noexcept;

/// Fórmula a partir de su identificador; vacío si no existe.
[[nodiscard]] std::optional<GfrFormula> gfrFormulaFromId(std::string_view formula_id) noexcept;

/// Crea el estimador de una fórmula.
[[nodiscard]] std::unique_ptr<GfrEstimator> makeGfrEstimator(GfrFormula formula);

}  // namespace calc
