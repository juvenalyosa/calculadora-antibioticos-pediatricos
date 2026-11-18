/// @file html.hpp
/// @brief Conversión de resultados del núcleo a HTML para mostrarlos en Qt.
///
/// Los números y las frases de dosis salen de calc/text.hpp, de modo que la
/// interfaz gráfica escribe exactamente lo mismo que la consola.
#pragma once

#include <string_view>
#include <vector>

#include <QString>

#include "calc/advisor.hpp"
#include "calc/dose_engine.hpp"
#include "calc/warnings.hpp"

namespace calc::gui {

/// Texto UTF-8 del núcleo → QString.
[[nodiscard]] QString toQString(std::string_view text);

/// Lista de advertencias como avisos con franja de color según su gravedad.
/// Los colores salen del tema en uso (theme.hpp).
[[nodiscard]] QString warningsHtml(const std::vector<Warning>& warnings);

/// Un aviso con contenido HTML propio (ya escapado), con el color de la gravedad.
[[nodiscard]] QString noticeHtml(Severity severity, const QString& body_html);

/// Ficha de una enfermedad para el paso 3: si lleva antibiótico, sus líneas y
/// opciones de tratamiento y la guía principal.
[[nodiscard]] QString diseasePreviewHtml(const KnowledgeBase& kb, const Disease& disease);

/// Plan de tratamiento completo como HTML.
[[nodiscard]] QString planHtml(const CalculationRequest& request, const TreatmentPlan& plan,
                               const std::vector<Annotation>& annotations);

}  // namespace calc::gui
