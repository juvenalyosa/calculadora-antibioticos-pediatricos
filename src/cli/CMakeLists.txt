# calc-dosis: interfaz de consola sobre calc_core.
# Sirve para validar la base de conocimiento, hacer cálculos de comprobación
# y automatizar pruebas; no es la interfaz para el usuario clínico.

add_executable(calc-dosis
  main.cpp
  plan_printer.cpp)

target_link_libraries(calc-dosis PRIVATE calc::core)
calc_set_warnings(calc-dosis)
