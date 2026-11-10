/// @file renal.hpp
/// @brief Función renal del paciente y estimación de la TFG.
///
/// La estimación está detrás de la interfaz `GfrEstimator` para poder añadir
/// otras fórmulas (p. ej. con cistatina C) sin tocar el motor de dosis.
#pragma once

#include <optional>
#include <string>
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

}  // namespace calc
