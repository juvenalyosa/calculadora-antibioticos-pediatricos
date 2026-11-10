/// @file html.hpp
/// @brief Conversión de resultados del núcleo a HTML para mostrarlos en Qt.
///
/// Los números y las frases de dosis salen de calc/text.hpp, de modo que la
/// interfaz gráfica escribe exactamente lo mismo que la consola.
#pragma once

#include <string_view>
#include <vector>

#include <QPalette>
#include <QString>

#include "calc/advisor.hpp"
#include "calc/dose_engine.hpp"
#include "calc/warnings.hpp"

namespace calc::gui {

/// Texto UTF-8 del núcleo → QString.
[[nodiscard]] QString toQString(std::string_view text);

/// Lista de advertencias como HTML, con color según su gravedad. La paleta
/// se usa para elegir colores legibles en modo claro y en modo oscuro.
[[nodiscard]] QString warningsHtml(const std::vector<Warning>& warnings, const QPalette& palette);

/// Plan de tratamiento completo como HTML.
[[nodiscard]] QString planHtml(const CalculationRequest& request, const TreatmentPlan& plan,
                               const std::vector<Annotation>& annotations,
                               const QPalette& palette);

}  // namespace calc::gui
