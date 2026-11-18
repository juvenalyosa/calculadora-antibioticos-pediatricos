/// @file theme.hpp
/// @brief Aspecto visual de la interfaz: colores, hoja de estilo e icono.
///
/// Todos los colores salen de una sola tabla (Theme), en versión clara y
/// oscura. Los widgets se pintan con la hoja de estilo que genera applyTheme()
/// y el resultado en HTML usa la misma tabla, de modo que ambos coinciden.
#pragma once

#include <QIcon>
#include <QString>

class QApplication;

namespace calc::gui {

/// Colores de la interfaz, como texto «#rrggbb».
struct Theme {
  bool dark = false;

  QString window;       ///< Fondo de la ventana.
  QString surface;      ///< Fondo de tarjetas y campos.
  QString surface_alt;  ///< Fondo secundario: filas alternas, hover, desglose.
  QString border;       ///< Bordes y separadores.
  QString text;         ///< Texto principal.
  QString muted;        ///< Texto secundario.
  QString disabled;     ///< Texto deshabilitado.

  QString primary;          ///< Color de marca: acciones y dosis.
  QString primary_hover;
  QString primary_pressed;
  QString on_primary;       ///< Texto sobre el color de marca.
  QString primary_soft;     ///< Fondo suave del color de marca.

  QString sidebar;          ///< Fondo de la barra lateral de pasos.
  QString sidebar_text;
  QString sidebar_muted;
  QString sidebar_active;   ///< Fondo del paso actual.

  // Avisos: texto, fondo y franja de acento por gravedad.
  QString info_text, info_bg, info_accent;
  QString caution_text, caution_bg, caution_accent;
  QString danger_text, danger_bg, danger_accent;
  QString success_text, success_bg, success_accent;

  // Franja que distingue cada línea de tratamiento en el resultado.
  QString line_first, line_second, line_allergy;
};

enum class ThemeChoice { System, Light, Dark };

/// Interpreta «claro», «oscuro» o «sistema» (cualquier otro texto = sistema).
[[nodiscard]] ThemeChoice themeChoiceFromText(const QString& text);

/// Fija el estilo Fusion (mismo aspecto en Windows, macOS y Linux), la paleta
/// y la hoja de estilo. Debe llamarse una vez, antes de crear las ventanas.
void applyTheme(QApplication& app, ThemeChoice choice);

/// Tema en uso (el claro hasta que se llame applyTheme).
[[nodiscard]] const Theme& theme();

/// Icono de la aplicación, dibujado por código (no necesita archivos).
[[nodiscard]] QIcon makeAppIcon();

}  // namespace calc::gui
