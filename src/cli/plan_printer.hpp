/// @file plan_printer.hpp
/// @brief Escritura de un plan de tratamiento como texto para la consola.
#pragma once

#include <iosfwd>

#include "calc/dose_engine.hpp"

namespace calc::cli {

struct PrintOptions {
  bool show_trace = true;  ///< Incluir el cálculo desglosado paso a paso.
};

/// Escribe el plan completo: advertencias, líneas, opciones y dosis.
void printPlan(std::ostream& out, const CalculationRequest& request, const TreatmentPlan& plan,
               const PrintOptions& options);

}  // namespace calc::cli
