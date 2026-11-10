#include <doctest/doctest.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "calc/error_report.hpp"
#include "calc/version.hpp"
#include "support/test_kb.hpp"

using namespace calc;

TEST_CASE("reporte: captura automática de los parámetros del cálculo") {
  Patient infant = testing::infantCase();
  infant.sex = Sex::Female;
  const CalculationRequest request{infant,
                                   RenalFunction::estimated(Gfr(77.4), "Schwartz de cabecera"),
                                   "oma"};
  const ErrorReport report = makeErrorReport("resultado", ReportCategory::CalculationError,
                                             "La dosis me parece alta.", request, "1.2.3");

  CHECK(report.step == "resultado");
  CHECK(report.comment == "La dosis me parece alta.");
  CHECK(report.kb_version == "1.2.3");
  CHECK(report.app_version == std::string(kAppVersion));
  // ISO 8601 en UTC: "AAAA-MM-DDTHH:MM:SSZ".
  REQUIRE(report.timestamp_utc.size() == 20);
  CHECK(report.timestamp_utc.back() == 'Z');

  CHECK(report.context.at("edad_meses") == "12");
  CHECK(report.context.at("peso_kg") == "10");
  CHECK(report.context.at("talla_cm") == "75");
  CHECK(report.context.at("sexo") == "femenino");
  CHECK(report.context.at("funcion_renal") == "estimada");
  CHECK(report.context.at("tfg") == "77.4");
  CHECK(report.context.at("metodo_tfg") == "Schwartz de cabecera");
  CHECK(report.context.at("enfermedad_id") == "oma");
}

TEST_CASE("reporte: no incluye lo que el usuario no introdujo") {
  Patient patient = testing::infantCase();
  patient.height.reset();
  const ErrorReport report = makeErrorReport("paciente", ReportCategory::Usability, "",
                                             {patient, RenalFunction::assumedNormal(), ""}, "x");
  CHECK(report.context.count("talla_cm") == 0);
  CHECK(report.context.count("sexo") == 0);
  CHECK(report.context.count("tfg") == 0);
  CHECK(report.context.count("enfermedad_id") == 0);
  CHECK(report.context.at("funcion_renal") == "asumida_normal");
}

TEST_CASE("reporte: el archivo JSONL acumula una línea por reporte") {
  const std::filesystem::path path =
      std::filesystem::temp_directory_path() / "calc_prueba_reportes.jsonl";
  std::filesystem::remove(path);

  JsonlFileSink sink(path);
  const CalculationRequest request{testing::adolescentCase(), RenalFunction::entered(Gfr(40.0)),
                                   "faringoamigdalitis"};
  sink.submit(makeErrorReport("resultado", ReportCategory::ClinicalContent, "Guía «nueva».",
                              request, "1.0"));
  sink.submit(makeErrorReport("renal", ReportCategory::Other, "Segundo reporte.", request, "1.0"));

  std::ifstream file(path);
  std::vector<nlohmann::json> lines;
  for (std::string line; std::getline(file, line);) {
    lines.push_back(nlohmann::json::parse(line));
  }
  file.close();
  std::filesystem::remove(path);

  REQUIRE(lines.size() == 2);
  CHECK(lines[0]["paso"] == "resultado");
  CHECK(lines[0]["categoria"] == "contenido_clinico");
  CHECK(lines[0]["comentario"] == "Guía «nueva».");  // el texto UTF-8 se conserva
  CHECK(lines[0]["parametros"]["peso_kg"] == "50");
  CHECK(lines[0]["parametros"]["tfg"] == "40");
  CHECK(lines[1]["paso"] == "renal");
}

TEST_CASE("reporte: falla de forma explícita si no se puede escribir") {
  JsonlFileSink sink("carpeta/que/no/existe/reportes.jsonl");
  const CalculationRequest request{testing::infantCase(), RenalFunction::assumedNormal(), "oma"};
  CHECK_THROWS_AS(sink.submit(makeErrorReport("paciente", ReportCategory::Other, "", request, "x")),
                  std::runtime_error);
}
