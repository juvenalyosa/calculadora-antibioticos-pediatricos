# =============================================================================
# Calculadora de dosis de antibióticos para enfermedades infecciosas
# prevalentes en la infancia — Universidad Militar Nueva Granada (UMNG)
#
# Archivo raíz de construcción. Estructura de objetivos:
#   calc_core   biblioteca con todo el dominio (sin interfaz gráfica)
#   calc-dosis  ejecutable de consola (validación, pruebas manuales, CI)
#   calc-gui    interfaz gráfica por pasos (solo si se encuentra Qt 6)
#   calc_tests  pruebas automáticas (incluye la matriz de validación alfa)
# =============================================================================
cmake_minimum_required(VERSION 3.21)

project(CalculadoraAntibioticos
  VERSION 0.2.0
  DESCRIPTION "Calculadora de dosis de antibióticos pediátricos (UMNG)"
  LANGUAGES CXX)

# --- Estándar del lenguaje ---------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # para clangd / clang-tidy

# --- Opciones de construcción ------------------------------------------------
option(CALC_BUILD_CLI "Construir el ejecutable de consola calc-dosis" ON)
option(CALC_BUILD_GUI "Construir la interfaz gráfica calc-gui (requiere Qt 6)" ON)
option(CALC_BUILD_TESTS "Construir y registrar las pruebas automáticas" ON)
option(CALC_WARNINGS_AS_ERRORS "Tratar las advertencias del compilador como errores" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

# --- Subproyectos ------------------------------------------------------------
add_subdirectory(third_party)
add_subdirectory(src/core)

if(CALC_BUILD_CLI)
  add_subdirectory(src/cli)
endif()

if(CALC_BUILD_GUI)
  # Qt no viene con el repositorio. Si no se encuentra, se construye todo lo
  # demás; para indicarle a CMake dónde está: -DCMAKE_PREFIX_PATH=<ruta de Qt>.
  find_package(Qt6 6.5 QUIET COMPONENTS Widgets)
  if(Qt6_FOUND)
    add_subdirectory(src/gui)
  else()
    message(STATUS "Qt 6 no encontrado: se omite la interfaz gráfica (calc-gui).")
  endif()
endif()

if(WIN32)
  # Bibliotecas de ejecución de Visual C++ junto al ejecutable: el programa
  # funciona sin instalar nada más ni pedir permisos de administrador.
  set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
  include(InstallRequiredSystemLibraries)
endif()

if(CALC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
