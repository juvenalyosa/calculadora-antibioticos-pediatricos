/// @file ui.hpp
/// @brief Piezas de diseño comunes a todas las páginas.
#pragma once

#include <QString>

class QFrame;
class QLabel;
class QWidget;

namespace calc::gui {

/// Encabezado de página: banda de color con el título, una línea de ayuda y
/// el progreso. `step` es el índice del paso (0 = primero); con un valor
/// negativo no se dibujan los pasos (diálogos).
[[nodiscard]] QWidget* makePageHeader(int step, const QString& title, const QString& subtitle,
                                      QWidget* parent);

/// Tarjeta: marco blanco con borde y esquinas redondeadas. Su contenido se
/// agrega con un layout propio.
[[nodiscard]] QFrame* makeCard(QWidget* parent, const char* object_name = "card");

/// Etiqueta de campo (pequeña, gris, en negrilla).
[[nodiscard]] QLabel* makeFieldLabel(const QString& text, QWidget* parent);

/// Texto de ayuda bajo un campo.
[[nodiscard]] QLabel* makeHint(const QString& text, QWidget* parent);

/// Cambia una propiedad usada por la hoja de estilo y repinta el widget.
void setStyleProperty(QWidget* widget, const char* name, bool value);

}  // namespace calc::gui
