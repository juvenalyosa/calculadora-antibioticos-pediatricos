/// @file advisor.hpp
/// @brief Punto de extensión para módulos de apoyo (incluida una futura IA).
///
/// Un `Advisor` observa un cálculo ya terminado y puede añadir OBSERVACIONES.
/// Es el lugar previsto para integrar más adelante modelos de inteligencia
/// artificial o reglas adicionales sin tocar el motor.
///
/// Contrato (lo garantiza la forma de la interfaz, no la buena voluntad):
///   - SOLO LECTURA: recibe el plan como `const`; no puede cambiar una dosis.
///   - NO BLOQUEA: si un módulo falla, se registra y el cálculo sigue intacto.
///   - ATRIBUIDO: cada observación lleva el identificador del módulo que la
///     emitió, para que la interfaz la muestre separada del cálculo validado.
///
/// Ejemplos de módulos posibles: detección de interacciones, comparación con
/// una segunda fuente, clasificación de reportes de error, explicación en
/// lenguaje natural del desglose.
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "calc/dose_engine.hpp"
#include "calc/warnings.hpp"

namespace calc {

/// Observación emitida por un módulo de apoyo.
struct Annotation {
  std::string advisor_id;  ///< Lo completa el registro; el módulo no lo fija.
  Severity severity = Severity::Info;
  std::string message;
};

/// Lo que un módulo puede ver: la solicitud y el plan ya calculado.
struct AdvisorContext {
  const CalculationRequest& request;
  const TreatmentPlan& plan;
};

/// Interfaz de un módulo de apoyo.
class Advisor {
 public:
  virtual ~Advisor() = default;
  /// Identificador estable del módulo (p. ej. "interacciones", "ia-revision").
  [[nodiscard]] virtual std::string id() const = 0;
  [[nodiscard]] virtual std::vector<Annotation> review(const AdvisorContext& context) const = 0;
};

/// Conjunto de módulos activos.
class AdvisorRegistry {
 public:
  void add(std::unique_ptr<Advisor> advisor);
  [[nodiscard]] std::size_t size() const noexcept { return advisors_.size(); }

  /// Ejecuta todos los módulos. Una excepción dentro de un módulo se convierte
  /// en una observación informativa de ese módulo; nunca se propaga.
  [[nodiscard]] std::vector<Annotation> reviewAll(const AdvisorContext& context) const;

 private:
  std::vector<std::unique_ptr<Advisor>> advisors_;
};

}  // namespace calc
