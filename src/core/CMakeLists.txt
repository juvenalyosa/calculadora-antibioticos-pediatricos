# calc_core: todo el dominio de la calculadora, sin dependencias de interfaz.
# Cualquier interfaz (consola, Qt, futura versión web) enlaza con este objetivo.

# La versión de la aplicación se toma de project(VERSION ...) del archivo raíz.
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/calc/version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/generated/calc/version.hpp"
  @ONLY)

add_library(calc_core STATIC
  src/advisor.cpp
  src/dose_engine.cpp
  src/error_report.cpp
  src/kb_loader.cpp
  src/knowledge_base.cpp
  src/patient.cpp
  src/renal.cpp
  src/text.cpp)

add_library(calc::core ALIAS calc_core)

target_include_directories(calc_core PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_BINARY_DIR}/generated")

# nlohmann/json es un detalle de implementación: no aparece en las cabeceras
# públicas, por eso se enlaza como PRIVATE.
target_link_libraries(calc_core PRIVATE calc_third_party_json)

calc_set_warnings(calc_core)
