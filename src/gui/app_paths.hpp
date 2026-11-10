/// @file app_paths.hpp
/// @brief Ubicación de los archivos que usa la interfaz gráfica.
#pragma once

#include <QString>

namespace calc::gui {

/// Busca la base de conocimiento, en este orden:
///   1. la ruta dada explícitamente (opción --kb), si no está vacía;
///   2. la variable de entorno CALC_KB_FILE;
///   3. junto a la aplicación instalada (Resources en macOS,
///      data/kb/ al lado del ejecutable en Windows y Linux);
///   4. el árbol de fuentes (solo sirve durante el desarrollo).
/// Devuelve la primera que exista, o una cadena vacía.
[[nodiscard]] QString locateKnowledgeBase(const QString& explicit_path);

/// Archivo local donde se acumulan los reportes de error del usuario.
/// Crea la carpeta de datos de la aplicación si no existe.
[[nodiscard]] QString reportFilePath();

}  // namespace calc::gui
