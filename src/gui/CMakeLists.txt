# calc-gui: interfaz gráfica por pasos (Qt 6 Widgets) sobre calc_core.
#
# Solo se construye si CMake encuentra Qt 6 (ver el CMakeLists.txt raíz).
# Toda la lógica clínica está en calc_core: aquí solo se piden datos y se
# muestran resultados.

set(CMAKE_AUTOMOC ON)

set(CALC_KB_FILE "${PROJECT_SOURCE_DIR}/data/kb/base_conocimiento.json")

qt_add_executable(calc-gui MACOSX_BUNDLE WIN32
  main.cpp
  app_paths.cpp
  app_paths.hpp
  calculator_wizard.cpp
  calculator_wizard.hpp
  html.cpp
  html.hpp
  report_dialog.cpp
  report_dialog.hpp
  screenshot_tour.cpp
  screenshot_tour.hpp
  session.hpp
  pages/disease_page.cpp
  pages/disease_page.hpp
  pages/patient_page.cpp
  pages/patient_page.hpp
  pages/renal_page.cpp
  pages/renal_page.hpp
  pages/result_page.cpp
  pages/result_page.hpp
  "${CALC_KB_FILE}")

# En macOS la base de conocimiento viaja dentro del paquete .app.
set_source_files_properties("${CALC_KB_FILE}" PROPERTIES
  MACOSX_PACKAGE_LOCATION Resources
  HEADER_FILE_ONLY ON)

target_include_directories(calc-gui PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(calc-gui PRIVATE calc::core Qt6::Widgets)

# Ruta de la base dentro del árbol de fuentes: último recurso al ejecutar
# desde el directorio de construcción durante el desarrollo.
target_compile_definitions(calc-gui PRIVATE CALC_DEV_KB_FILE="${CALC_KB_FILE}")

set_target_properties(calc-gui PROPERTIES
  OUTPUT_NAME "CalculadoraAntibioticos"
  MACOSX_BUNDLE_BUNDLE_NAME "Calculadora de antibióticos"
  MACOSX_BUNDLE_GUI_IDENTIFIER "co.edu.unimilitar.calculadora-antibioticos"
  MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")

calc_set_warnings(calc-gui)
