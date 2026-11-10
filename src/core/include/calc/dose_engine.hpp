/// @file dose_engine.hpp
/// @brief Motor de cálculo de dosis.
///
/// Principios de diseño:
///   1. DETERMINISTA: mismos datos de entrada y misma base de conocimiento
///      producen siempre el mismo resultado. Ningún módulo externo (incluida
///      una futura IA, ver advisor.hpp) puede modificar una dosis calculada.
///   2. TRANSPARENTE: cada resultado trae el cálculo desglosado paso a paso
///      (`trace`) y la fuente, para que el usuario lo pueda verificar a mano.
///   3. SIN FALLOS SILENCIOSOS: topes, ajustes, datos no verificados y
///      redondeos relevantes se informan como advertencias.
///
/// Orden del cálculo para cada medicamento (ver docs/arquitectura.md):
///   dosis base → topes máximos → ajuste renal → conversión a presentación.
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "calc/knowledge_base.hpp"
#include "calc/patient.hpp"
#include "calc/renal.hpp"
#include "calc/warnings.hpp"

namespace calc {

/// Todo lo que el usuario introduce para un cálculo.
struct CalculationRequest {
  Patient patient;
  RenalFunction renal;
  std::string disease_id;
};

/// Un paso del cálculo desglosado.
struct TraceStep {
  std::string title;   ///< P. ej. "Dosis diaria".
  std::string detail;  ///< P. ej. "90 mg/kg/día × 10 kg = 900 mg/día".
};

/// Cómo administrar una dosis con una presentación concreta.
struct Administration {
  std::string presentation_id;
  std::string presentation;     ///< Descripción de la presentación.
  std::string dispensing_unit;  ///< "mL", "tableta"...
  double exact_quantity = 0.0;    ///< Cantidad exacta antes de redondear.
  double rounded_quantity = 0.0;  ///< Cantidad medible o partible.
  double delivered_amount = 0.0;  ///< Principio activo que se entrega realmente.
  double deviation = 0.0;         ///< (entregado − calculado) / calculado.
  /// false si el redondeo se aleja demasiado de la dosis calculada.
  bool suitable = true;
};

namespace limits {
/// Desviación máxima aceptable entre la dosis calculada y la que realmente se
/// entrega tras redondear a la presentación.
inline constexpr double kMaxRoundingDeviation = 0.10;
}  // namespace limits

/// Referencia a la fuente de un dato, lista para mostrar.
struct SourceRef {
  std::string id;
  std::string title;
  std::string organization;
  int year = 0;
  std::string page;
};

/// Resultado para un medicamento de una opción terapéutica.
struct RegimenResult {
  std::string drug_id;
  std::string drug_name;
  DrugUnit unit = DrugUnit::Milligram;
  Route route = Route::Oral;

  /// false para indicaciones sin cálculo (ver `note`).
  bool calculated = false;
  /// true si el ajuste renal indica evitar el medicamento: NO administrar.
  bool not_recommended = false;

  double dose_per_administration = 0.0;  ///< Dosis final por toma.
  std::optional<int> interval_hours;     ///< Vacío = dosis única.
  std::optional<double> daily_dose;      ///< Vacío si es dosis única.

  /// Dosis por toma con los extremos del rango de la guía (tras topes, antes
  /// del ajuste renal).
  std::optional<double> range_min_per_dose;
  std::optional<double> range_max_per_dose;

  bool max_cap_applied = false;
  bool renal_adjustment_applied = false;

  std::optional<int> duration_min_days;
  std::optional<int> duration_max_days;
  std::string note;

  SourceRef source;
  ReviewStatus status = ReviewStatus::Sample;

  std::vector<Administration> administrations;  ///< Una por presentación compatible.
  std::vector<TraceStep> trace;
  std::vector<Warning> warnings;
};

struct OptionResult {
  int number = 0;
  std::string description;
  std::vector<RegimenResult> components;
};

/// Opción de la guía que no aplica a este paciente (por edad o peso).
struct SkippedOption {
  int number = 0;
  std::string description;
  std::string reason;
};

struct LineResult {
  TreatmentLineKind kind = TreatmentLineKind::First;
  std::vector<OptionResult> options;
  std::vector<SkippedOption> skipped;
};

/// Plan completo para una enfermedad: todas las líneas con sus opciones.
struct TreatmentPlan {
  std::string disease_id;
  std::string disease_name;
  std::string kb_version;

  /// true si los datos del paciente impiden calcular (ver `warnings`).
  bool blocked = false;
  bool antibiotic_indicated = true;
  std::string message;  ///< Mensaje de la enfermedad (p. ej. "no requiere antibiótico").

  std::vector<Warning> warnings;  ///< Advertencias a nivel de paciente.
  std::vector<LineResult> lines;
};

/// Motor de cálculo. No guarda estado entre llamadas; la base de conocimiento
/// debe vivir al menos tanto como el motor.
class DoseEngine {
 public:
  explicit DoseEngine(const KnowledgeBase& kb) noexcept : kb_(kb) {}

  /// Calcula todas las líneas de manejo de la enfermedad para el paciente.
  /// @throws NotFoundError si `request.disease_id` no existe en la base.
  [[nodiscard]] TreatmentPlan plan(const CalculationRequest& request) const;

  /// Calcula un único esquema. Útil para pruebas y para interfaces que
  /// recalculan una sola tarjeta.
  [[nodiscard]] RegimenResult calculate(const Patient& patient, const RenalFunction& renal,
                                        const Regimen& regimen) const;

 private:
  const KnowledgeBase& kb_;
};

}  // namespace calc
