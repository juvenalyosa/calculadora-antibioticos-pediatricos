# Bibliotecas de terceros incluidas en el repositorio (ver third_party/README.md).
# Se exponen como objetivos INTERFACE con inclusión SYSTEM para silenciar sus
# advertencias.

add_library(calc_third_party_json INTERFACE)
target_include_directories(calc_third_party_json SYSTEM INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json")

add_library(calc_third_party_doctest INTERFACE)
target_include_directories(calc_third_party_doctest SYSTEM INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/doctest")
