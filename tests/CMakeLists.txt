# Pruebas automáticas.
#
#   pruebas_unitarias  cada módulo del núcleo con una base de conocimiento
#                      SINTÉTICA (tests/support/test_kb.hpp), independiente
#                      del contenido clínico real.
#   matriz_alfa        dentro del mismo ejecutable: compara el motor contra
#                      tests/data/matriz_alfa.csv usando la base real.
#   kb_al_dia          comprueba que el JSON corresponde a las hojas CSV.

add_executable(calc_tests
  test_main.cpp
  test_advisor.cpp
  test_alpha_matrix.cpp
  test_dose_engine.cpp
  test_error_report.cpp
  test_kb_loader.cpp
  test_patient.cpp
  test_renal.cpp
  test_text.cpp
  test_units.cpp)

target_include_directories(calc_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(calc_tests PRIVATE
  calc::core
  calc_third_party_doctest
  calc_third_party_json)

# Rutas a los datos, para que las pruebas no dependan del directorio actual.
target_compile_definitions(calc_tests PRIVATE
  CALC_KB_FILE="${PROJECT_SOURCE_DIR}/data/kb/base_conocimiento.json"
  CALC_ALPHA_MATRIX_FILE="${CMAKE_CURRENT_SOURCE_DIR}/data/matriz_alfa.csv")

calc_set_warnings(calc_tests)

add_test(NAME pruebas_unitarias COMMAND calc_tests)

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
  add_test(NAME kb_al_dia
    COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/construir_kb.py" --check)
endif()
