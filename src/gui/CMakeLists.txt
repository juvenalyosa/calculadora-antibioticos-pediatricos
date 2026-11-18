# calc-gui: interfaz gráfica por pasos (Qt 6 Widgets) sobre calc_core.
#
# Solo se construye si CMake encuentra Qt 6 (ver el CMakeLists.txt raíz).
# Toda la lógica clínica está en calc_core: aquí solo se piden datos y se
# muestran resultados.

set(CMAKE_AUTOMOC ON)

set(CALC_KB_FILE "${PROJECT_SOURCE_DIR}/data/kb/base_conocimiento.json")

# Icono del ejecutable. Los archivos se generan con la opción
# --exportar-icono de la propia aplicación (ver docs/empaquetado.md).
set(CALC_ICON_DIR "${PROJECT_SOURCE_DIR}/packaging/iconos")
set(CALC_PLATFORM_SOURCES)
if(APPLE)
  set(CALC_MACOS_ICON "${CALC_ICON_DIR}/CalculadoraAntibioticos.icns")
  list(APPEND CALC_PLATFORM_SOURCES "${CALC_MACOS_ICON}")
  set_source_files_properties("${CALC_MACOS_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
elseif(WIN32)
  list(APPEND CALC_PLATFORM_SOURCES "${PROJECT_SOURCE_DIR}/packaging/windows/recursos.rc")
endif()

qt_add_executable(calc-gui MACOSX_BUNDLE WIN32
  ${CALC_PLATFORM_SOURCES}
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
  theme.cpp
  theme.hpp
  widgets/age_band.cpp
  widgets/age_band.hpp
  widgets/hero_header.cpp
  widgets/hero_header.hpp
  widgets/ui.cpp
  widgets/ui.hpp
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
  MACOSX_BUNDLE_ICON_FILE "CalculadoraAntibioticos.icns"
  MACOSX_BUNDLE_GUI_IDENTIFIER "co.edu.unimilitar.calculadora-antibioticos"
  MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")

calc_set_warnings(calc-gui)

# --- Instalación -------------------------------------------------------------
# `cmake --install` deja lo que necesitan los instaladores, sin las bibliotecas
# de Qt: esas las añaden macdeployqt, windeployqt o linuxdeploy al empaquetar.
#   macOS    el paquete .app completo (la base va dentro, en Resources)
#   Windows  el ejecutable y data/kb/ en la misma carpeta
#   Linux    bin/, share/calculadora-antibioticos/, acceso directo e icono
include(GNUInstallDirs)
if(APPLE)
  install(TARGETS calc-gui BUNDLE DESTINATION .)
elseif(WIN32)
  install(TARGETS calc-gui RUNTIME DESTINATION .)
  install(FILES "${CALC_KB_FILE}" DESTINATION data/kb)
else()
  install(TARGETS calc-gui RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
  install(FILES "${CALC_KB_FILE}" DESTINATION "${CMAKE_INSTALL_DATADIR}/calculadora-antibioticos")
  install(FILES "${PROJECT_SOURCE_DIR}/packaging/linux/calculadora-antibioticos.desktop"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/applications")
  install(FILES "${CALC_ICON_DIR}/calculadora-antibioticos.png"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps")
endif()
