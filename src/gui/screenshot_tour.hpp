/// @file screenshot_tour.hpp
/// @brief Recorrido automático que guarda una captura de cada paso.
///
/// Sirve para dos cosas: comprobar la interfaz sin manejarla a mano, y
/// regenerar las imágenes del manual de usuario cada vez que cambie la
/// interfaz (opción --capturas <carpeta>).
#pragma once

#include <QString>

#include "calc/error_report.hpp"
#include "calculator_wizard.hpp"

namespace calc::gui {

/// Recorre el asistente con el caso tipo 1 de la validación alfa (lactante de
/// 1 año, 10 kg, 75 cm, otitis media aguda) y guarda las capturas en `folder`.
/// @return true si se guardaron todas las imágenes.
[[nodiscard]] bool runScreenshotTour(CalculatorWizard& wizard, ReportSink& sink,
                                     const QString& folder);

}  // namespace calc::gui
