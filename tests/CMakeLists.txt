# Pruebas automáticas.
#
#   pruebas_unitarias  cada módulo del núcleo con una base de conocimiento
#                      SINTÉTICA (tests/support/test_kb.hpp), independiente
#                      del contenido clínico real.
#   matriz_alfa        dentro del mismo ejecutable: compara el motor contra
#                      tests/data/matriz_alfa.csv usando la base real.
#   kb_al_dia          comprueba que el JSON corresponde a las hojas CSV.
#   interfaz_recorrido recorre la interfaz gráfica (si se construyó con Qt).
#   validacion_poblacion prueba exhaustiva con la población modelada.

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

# Prueba de humo de la interfaz gráfica: recorre los cuatro pasos sin pantalla
# y guarda una captura de cada uno. Solo existe si se construyó calc-gui.
if(TARGET calc-gui)
  add_test(NAME interfaz_recorrido
    COMMAND calc-gui --capturas "${CMAKE_CURRENT_BINARY_DIR}/capturas")
  set_tests_properties(interfaz_recorrido PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
  # Prueba exhaustiva: población pediátrica modelada con los patrones de la OMS,
  # todas las enfermedades y escenarios renales, verificada contra un recálculo
  # independiente y reglas de seguridad (docs/validacion/LEEME.md).
  if(TARGET calc-dosis)
    add_test(NAME validacion_poblacion
      COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/docs/validacion/ejecutar.py"
              $<TARGET_FILE:calc-dosis> "${CMAKE_CURRENT_BINARY_DIR}/validacion")
    set_tests_properties(validacion_poblacion PROPERTIES TIMEOUT 900)
  endif()

  add_test(NAME kb_al_dia
    COMMAND "${Python3_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/tools/construir_kb.py" --check)
endif()
