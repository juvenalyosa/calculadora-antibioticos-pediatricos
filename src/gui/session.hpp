/// @file session.hpp
/// @brief Estado de un cálculo en curso dentro de la interfaz gráfica.
#pragma once

#include <optional>
#include <string>

#include "calc/dose_engine.hpp"

namespace calc::gui {

/// Lo que el usuario ha introducido hasta ahora.
///
/// Cada página escribe aquí en cuanto cambia un campo (no solo al pulsar
/// «Siguiente»), de modo que un reporte de error hecho en cualquier paso
/// capture exactamente lo que el usuario tiene en pantalla.
struct Session {
  Patient patient{Age::fromMonths(0), Kilograms(0.0), std::nullopt};
  RenalFunction renal = RenalFunction::assumedNormal();
  std::string disease_id;

  [[nodiscard]] CalculationRequest request() const { return {patient, renal, disease_id}; }
};

}  // namespace calc::gui
