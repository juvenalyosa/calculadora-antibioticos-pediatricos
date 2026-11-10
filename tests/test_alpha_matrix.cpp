// Validación alfa automatizada.
//
// La metodología del proyecto (van der Zanden et al., 2020) pide calcular con
// dos pacientes tipo —lactante de 1 año, 10 kg, 75 cm y adolescente de 13
// años, 50 kg, 162 cm— la primera y la segunda línea de cada enfermedad, con
// y sin ajuste renal, y repetir hasta que no haya ningún error.
//
// Aquí esa matriz es un archivo: tests/data/matriz_alfa.csv. Cada fila es un
// cálculo hecho A MANO por el equipo clínico; esta prueba ejecuta el motor
// con la base de conocimiento real y exige que coincida. "Sin ningún error"
// equivale a que esta prueba pase. Ver docs/validacion_alfa.md.

#include <doctest/doctest.h>

#include <fstream>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "calc/dose_engine.hpp"
#include "calc/kb_loader.hpp"

using namespace calc;

namespace {

using Row = std::map<std::string, std::string>;

/// Divide una línea CSV simple. La matriz no admite comas dentro de un campo.
std::vector<std::string> splitCsvLine(const std::string& line) {
  std::vector<std::string> fields;
  std::string field;
  std::istringstream stream(line);
  while (std::getline(stream, field, ',')) {
    fields.push_back(field);
  }
  if (!line.empty() && line.back() == ',') {
    fields.emplace_back();  // último campo vacío
  }
  return fields;
}

std::vector<Row> readMatrix(const std::string& path) {
  std::ifstream file(path);
  REQUIRE_MESSAGE(file.good(), "no se pudo abrir la matriz alfa: " << path);

  std::string line;
  std::getline(file, line);
  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }
  const std::vector<std::string> header = splitCsvLine(line);

  std::vector<Row> rows;
  while (std::getline(file, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();  // archivo guardado en Windows
    }
    if (line.empty()) {
      continue;
    }
    const std::vector<std::string> fields = splitCsvLine(line);
    REQUIRE_MESSAGE(fields.size() == header.size(),
                    "fila con número de columnas incorrecto: " << line);
    Row row;
    for (std::size_t i = 0; i < header.size(); ++i) {
      row[header[i]] = fields[i];
    }
    rows.push_back(std::move(row));
  }
  return rows;
}

double toNumber(const std::string& text) {
  std::istringstream stream(text);
  stream.imbue(std::locale::classic());
  double value = 0.0;
  stream >> value;
  REQUIRE_MESSAGE(!stream.fail(), "valor no numérico en la matriz: «" << text << "»");
  return value;
}

TreatmentLineKind toLineKind(const std::string& text) {
  if (text == "primera") {
    return TreatmentLineKind::First;
  }
  if (text == "segunda") {
    return TreatmentLineKind::Second;
  }
  REQUIRE_MESSAGE(text == "alergia", "línea no reconocida en la matriz: «" << text << "»");
  return TreatmentLineKind::Allergy;
}

/// Busca en el plan el resultado que corresponde a la fila; nullptr si no está.
const RegimenResult* findResult(const TreatmentPlan& plan, const Row& row) {
  const TreatmentLineKind kind = toLineKind(row.at("linea"));
  const int option_number = static_cast<int>(toNumber(row.at("opcion")));
  for (const LineResult& line : plan.lines) {
    if (line.kind != kind) {
      continue;
    }
    for (const OptionResult& option : line.options) {
      if (option.number != option_number) {
        continue;
      }
      for (const RegimenResult& component : option.components) {
        if (component.drug_id == row.at("medicamento_id")) {
          return &component;
        }
      }
    }
  }
  return nullptr;
}

}  // namespace

TEST_CASE("validación alfa: el motor coincide con la matriz calculada a mano") {
  const KnowledgeBase kb = loadKnowledgeBaseFromFile(CALC_KB_FILE);
  const DoseEngine engine(kb);
  const std::vector<Row> rows = readMatrix(CALC_ALPHA_MATRIX_FILE);
  REQUIRE_FALSE(rows.empty());

  for (const Row& row : rows) {
    CAPTURE(row.at("caso"));  // identifica la fila si algo falla

    Patient patient{Age::fromMonths(static_cast<int>(toNumber(row.at("edad_meses")))),
                    Kilograms(toNumber(row.at("peso_kg"))), std::nullopt};
    if (!row.at("talla_cm").empty()) {
      patient.height = Centimeters(toNumber(row.at("talla_cm")));
    }
    const RenalFunction renal = row.at("tfg").empty()
                                    ? RenalFunction::assumedNormal()
                                    : RenalFunction::entered(Gfr(toNumber(row.at("tfg"))));

    const TreatmentPlan plan = engine.plan({patient, renal, row.at("enfermedad_id")});
    REQUIRE_FALSE(plan.blocked);

    const RegimenResult* result = findResult(plan, row);
    REQUIRE_MESSAGE(result != nullptr,
                    "el plan no contiene la línea, opción y medicamento de la fila");

    // Dosis por toma (tolerancia relativa del 0,1 %).
    CHECK(result->dose_per_administration ==
          doctest::Approx(toNumber(row.at("dosis_por_toma"))).epsilon(0.001));

    // Intervalo: vacío en la matriz significa dosis única.
    if (row.at("intervalo_horas").empty()) {
      CHECK_FALSE(result->interval_hours.has_value());
    } else {
      REQUIRE(result->interval_hours.has_value());
      CHECK(*result->interval_hours == static_cast<int>(toNumber(row.at("intervalo_horas"))));
    }

    // Cantidad a administrar con la presentación indicada (opcional).
    if (!row.at("presentacion_id").empty()) {
      const Administration* found = nullptr;
      for (const Administration& administration : result->administrations) {
        if (administration.presentation_id == row.at("presentacion_id")) {
          found = &administration;
        }
      }
      REQUIRE_MESSAGE(found != nullptr, "el resultado no incluye la presentación de la fila");
      CHECK(found->rounded_quantity ==
            doctest::Approx(toNumber(row.at("cantidad"))).epsilon(0.001));
    }
  }
}
