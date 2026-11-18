#include <doctest/doctest.h>

#include <algorithm>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "calc/kb_loader.hpp"
#include "support/test_kb.hpp"

using namespace calc;
using nlohmann::json;

namespace {

/// Copia modificable de la base sintética, para introducirle un defecto.
json validKb() {
  return json::parse(testing::kTestKbJson);
}

/// Carga la base y devuelve los problemas reportados (vacío si es válida).
std::vector<std::string> issuesOf(const json& kb) {
  try {
    (void)loadKnowledgeBaseFromString(kb.dump());
  } catch (const KnowledgeBaseError& error) {
    return error.issues();
  }
  return {};
}

/// Indica si algún problema contiene el fragmento de texto dado.
bool mentions(const std::vector<std::string>& issues, const std::string& fragment) {
  return std::any_of(issues.begin(), issues.end(), [&](const std::string& issue) {
    return issue.find(fragment) != std::string::npos;
  });
}

/// Acceso rápido al primer esquema de la base sintética (Fármaco A, mg/kg/día).
json& firstRegimen(json& kb) {
  return kb["enfermedades"][0]["lineas"][0]["opciones"][0]["componentes"][0];
}

}  // namespace

TEST_CASE("cargador: la base sintética se lee completa") {
  const KnowledgeBase kb = testing::loadTestKb();
  CHECK(kb.schema_version == 1);
  CHECK(kb.version == "prueba");
  CHECK(kb.sources.size() == 1);
  CHECK(kb.drugs.size() == 4);
  CHECK(kb.diseases.size() == 3);

  const Drug& drug = kb.drug("farmaco_a");
  CHECK(drug.unit == DrugUnit::Milligram);
  CHECK(drug.renal.kind == RenalAdjustmentKind::Banded);
  CHECK(drug.renal.bands.size() == 3);

  // Líquido: 250 mg en 5 mL → 50 mg/mL, se mide de 0,5 en 0,5 mL.
  REQUIRE(drug.presentations.size() == 2);
  CHECK(drug.presentations[0].amount_per_unit == doctest::Approx(50.0));
  CHECK(drug.presentations[0].dispensing_unit == "mL");
  CHECK(drug.presentations[0].rounding_step == doctest::Approx(0.5));
  // Sólido divisible: medias tabletas.
  CHECK(drug.presentations[1].amount_per_unit == doctest::Approx(500.0));
  CHECK(drug.presentations[1].rounding_step == doctest::Approx(0.5));

  const Regimen& regimen = kb.disease("enfermedad_x").lines[0].options[0].components[0];
  CHECK(regimen.basis == DoseBasis::PerKgPerDay);
  CHECK(regimen.dose_usual == doctest::Approx(50.0));
  CHECK(regimen.interval_hours == 8);
  CHECK(regimen.max_per_day == 1500.0);
  CHECK(regimen.status == ReviewStatus::Verified);
  CHECK(regimen.verified_by.size() == 2);

  CHECK(kb.findDisease("no_existe") == nullptr);
  CHECK_THROWS_AS((void)kb.disease("no_existe"), NotFoundError);
  CHECK_THROWS_AS((void)kb.drug("no_existe"), NotFoundError);
}

TEST_CASE("cargador: bandas renales semiabiertas [mínimo, máximo)") {
  const KnowledgeBase kb = testing::loadTestKb();
  const RenalAdjustment& renal = kb.drug("farmaco_a").renal;
  CHECK(renal.findBand(60.0) == nullptr);
  CHECK(renal.findBand(50.0) == nullptr);  // el máximo no pertenece a la banda
  REQUIRE(renal.findBand(30.0) != nullptr);
  CHECK(renal.findBand(30.0)->gfr_max == doctest::Approx(50.0));
  REQUIRE(renal.findBand(29.9) != nullptr);
  CHECK(renal.findBand(29.9)->min_interval_hours == 24);
  REQUIRE(renal.findBand(5.0) != nullptr);
  CHECK(renal.findBand(5.0)->avoid);
}

TEST_CASE("cargador: la base clínica del repositorio es válida") {
  const KnowledgeBase kb = loadKnowledgeBaseFromFile(CALC_KB_FILE);
  CHECK(kb.schema_version == kSupportedSchemaVersion);
  CHECK_FALSE(kb.diseases.empty());
}

TEST_CASE("cargador: errores de archivo y de sintaxis") {
  CHECK_THROWS_AS((void)loadKnowledgeBaseFromFile("no/existe.json"), KnowledgeBaseError);
  CHECK_THROWS_AS((void)loadKnowledgeBaseFromString("{ esto no es json"), KnowledgeBaseError);
  CHECK(mentions(issuesOf(json::array()), "se esperaba un objeto"));
}

TEST_CASE("cargador: versión de esquema incompatible") {
  json kb = validKb();
  kb["esquema_version"] = 99;
  CHECK(mentions(issuesOf(kb), "no es compatible"));
}

TEST_CASE("cargador: campos faltantes o de tipo incorrecto") {
  json kb = validKb();
  kb.erase("version");
  kb["medicamentos"][0].erase("nombre");
  kb["medicamentos"][0]["unidad"] = "gramos";
  firstRegimen(kb)["intervalo_horas"] = "ocho";
  const auto issues = issuesOf(kb);

  // Se reportan TODOS los problemas en una sola pasada.
  CHECK(mentions(issues, "falta el campo «version»"));
  CHECK(mentions(issues, "falta el campo «nombre»"));
  CHECK(mentions(issues, "valor «gramos» no reconocido"));
  CHECK(mentions(issues, "«intervalo_horas» debe ser un número entero"));
}

TEST_CASE("cargador: referencias a identificadores inexistentes") {
  json kb = validKb();
  firstRegimen(kb)["medicamento_id"] = "fantasma";
  CHECK(mentions(issuesOf(kb), "el medicamento «fantasma» no existe"));

  kb = validKb();
  firstRegimen(kb)["fuente_id"] = "fuente_fantasma";
  CHECK(mentions(issuesOf(kb), "la fuente «fuente_fantasma» no existe"));
}

TEST_CASE("cargador: identificadores repetidos") {
  json kb = validKb();
  kb["medicamentos"].push_back(kb["medicamentos"][2]);
  CHECK(mentions(issuesOf(kb), "identificador repetido «farmaco_c»"));
}

TEST_CASE("cargador: reglas de dosis") {
  json kb = validKb();
  firstRegimen(kb)["dosis"] = {{"min", 60}, {"usual", 50}, {"max", 40}};
  CHECK(mentions(issuesOf(kb), "dosis inválida"));

  kb = validKb();
  firstRegimen(kb).erase("intervalo_horas");
  const auto issues = issuesOf(kb);
  CHECK(mentions(issues, "«mg_kg_dia» necesita «intervalo_horas»"));
  CHECK(mentions(issues, "«max_por_dia» solo aplica si hay «intervalo_horas»"));

  kb = validKb();
  firstRegimen(kb)["edad_meses"] = {{"min", 24}, {"max", 12}};
  CHECK(mentions(issuesOf(kb), "min no puede ser mayor que max"));
}

TEST_CASE("cargador: reglas de ajuste renal") {
  json kb = validKb();
  kb["medicamentos"][0]["ajuste_renal"]["bandas"][0]["tfg_min"] = 20;  // se solapa con 10–30
  CHECK(mentions(issuesOf(kb), "se solapan"));

  kb = validKb();
  kb["medicamentos"][0]["ajuste_renal"]["bandas"][0]["factor_dosis"] = 1.5;
  CHECK(mentions(issuesOf(kb), "«factor_dosis» debe estar entre"));

  kb = validKb();
  kb["medicamentos"][0]["ajuste_renal"]["bandas"] = json::array();
  CHECK(mentions(issuesOf(kb), "necesita al menos una banda"));

  kb = validKb();
  kb["medicamentos"][0].erase("ajuste_renal");
  CHECK(mentions(issuesOf(kb), "falta el campo «ajuste_renal»"));
}

TEST_CASE("cargador: «verificado» exige dos verificadores distintos") {
  json kb = validKb();
  firstRegimen(kb)["verificado_por"] = {"AB"};
  CHECK(mentions(issuesOf(kb), "dos verificadores distintos"));

  kb = validKb();
  firstRegimen(kb)["verificado_por"] = {"AB", "AB"};
  CHECK(mentions(issuesOf(kb), "dos verificadores distintos"));
}

TEST_CASE("cargador: enfermedad sin antibiótico") {
  json kb = validKb();
  kb["enfermedades"][1].erase("mensaje");
  CHECK(mentions(issuesOf(kb), "necesita el campo «mensaje»"));

  kb = validKb();
  kb["enfermedades"][0]["lineas"] = json::array();
  CHECK(mentions(issuesOf(kb), "no tiene líneas de manejo"));
}
