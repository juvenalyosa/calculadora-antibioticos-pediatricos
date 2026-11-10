/// @file text.hpp
/// @brief Textos de presentación compartidos por todas las interfaces.
///
/// Se centralizan aquí para que la consola, la interfaz gráfica y los
/// reportes escriban los números y las dosis exactamente igual.
#pragma once

#include <optional>
#include <string>

#include "calc/dose_engine.hpp"
#include "calc/knowledge_base.hpp"
#include "calc/renal.hpp"

namespace calc {

/// Formatea un número al estilo usado en Colombia: coma decimal, sin ceros de
/// relleno, y miles separados por espacio a partir de 10 000 (norma SI, evita
/// la ambigüedad punto/coma): 4,5 · 900 · 4000 · 600 000 · 1 200 000.
[[nodiscard]] std::string formatNumber(double value, int max_decimals = 2);

/// Cantidad de principio activo con su unidad: "450 mg", "600 000 UI".
[[nodiscard]] std::string formatAmount(double value, DrugUnit unit);

/// Frecuencia: "cada 12 h" o "dosis única".
[[nodiscard]] std::string describeInterval(const std::optional<int>& interval_hours);

/// Resumen de la dosis: "450 mg cada 12 h (900 mg/día)".
[[nodiscard]] std::string describeDose(const RegimenResult& result);

/// Duración: "10 días", "5 a 10 días" o cadena vacía si no está definida.
[[nodiscard]] std::string describeDuration(const RegimenResult& result);

/// Cantidad a administrar: "9 mL", "0,5 vial", "2 tabletas".
[[nodiscard]] std::string describeAdministration(const Administration& administration);

/// Función renal: "TFG 77,4 mL/min/1,73 m² (Schwartz de cabecera)".
[[nodiscard]] std::string describeRenal(const RenalFunction& renal);

}  // namespace calc
