# =============================================================================
# Calculadora de dosis de antibióticos para enfermedades infecciosas
# prevalentes en la infancia — Universidad Militar Nueva Granada (UMNG)
#
# Archivo raíz de construcción. Estructura de objetivos:
#   calc_core   biblioteca con todo el dominio (sin interfaz gráfica)
#   calc-dosis  ejecutable de consola (validación, pruebas manuales, CI)
#   calc_tests  pruebas automáticas (incluye la matriz de validación alfa)
#
# La interfaz gráfica (Qt) se añadirá como src/gui y enlazará con calc_core.
# =============================================================================
cmake_minimum_required(VERSION 3.21)

project(CalculadoraAntibioticos
  VERSION 0.1.0
  DESCRIPTION "Calculadora de dosis de antibióticos pediátricos (UMNG)"
  LANGUAGES CXX)

# --- Estándar del lenguaje ---------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # para clangd / clang-tidy

# --- Opciones de construcción ------------------------------------------------
option(CALC_BUILD_CLI "Construir el ejecutable de consola calc-dosis" ON)
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

if(CALC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
