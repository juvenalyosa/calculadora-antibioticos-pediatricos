/// @file batch.hpp
/// @brief Cálculo por lotes: muchos pacientes y enfermedades de un archivo CSV.
///
/// Sirve para las pruebas exhaustivas con poblaciones modeladas: la salida es
/// un CSV con una fila por medicamento de cada opción, sin texto para humanos.
#pragma once

#include <string>

#include "calc/knowledge_base.hpp"

namespace calc::cli {

/// Lee escenarios de `input_path` y escribe los resultados en `output_path`.
///
/// Columnas de entrada (el orden no importa; las opcionales pueden faltar):
///   caso, edad_meses, peso_kg, talla_cm, sexo (m|f), tfg, creatinina_mg_dl,
///   formula (ckid-u25|schwartz), enfermedad_id (vacío = todas).
/// Los campos no admiten comas. Devuelve el número de escenarios procesados.
/// Lanza std::runtime_error si no puede leer o escribir, o si falta una
/// columna obligatoria.
int runBatch(const KnowledgeBase& kb, const std::string& input_path,
             const std::string& output_path);

}  // namespace calc::cli
