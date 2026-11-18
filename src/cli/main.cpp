/// @file main.cpp
/// @brief calc-dosis: interfaz de consola de la calculadora.
///
/// Órdenes:
///   validar   comprueba que la base de conocimiento sea válida
///   listar    muestra las enfermedades disponibles
///   calcular  calcula el plan de tratamiento para un paciente
///   version   muestra la versión del programa
///
/// Códigos de salida: 0 = correcto; 1 = error de uso o de datos;
/// 2 = el cálculo quedó bloqueado por los datos del paciente.

#include <algorithm>
#include <exception>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "calc/dose_engine.hpp"
#include "calc/kb_loader.hpp"
#include "calc/text.hpp"
#include "calc/version.hpp"
#include "batch.hpp"
#include "plan_printer.hpp"

namespace {

using namespace calc;

constexpr const char* kDefaultKbPath = "data/kb/base_conocimiento.json";

constexpr const char* kUsage = R"(Uso:
  calc-dosis validar  [--kb <archivo>]
  calc-dosis listar   [--kb <archivo>]
  calc-dosis calcular [--kb <archivo>] --enfermedad <id>
                      (--edad-meses <n> | --edad-anios <n>) --peso-kg <x>
                      [--talla-cm <x>] [--sexo <m|f>]
                      [--tfg <x> | --creatinina <mg/dL>]
                      [--formula <ckid-u25|schwartz>] [--sin-desglose]
  calc-dosis lote     [--kb <archivo>] --entrada <escenarios.csv> --salida <resultados.csv>
  calc-dosis version

Si no se indica --kb se usa data/kb/base_conocimiento.json.
Con --creatinina la TFG se estima con la fórmula indicada en --formula:
  ckid-u25  (por defecto) necesita --talla-cm y --sexo
  schwartz  necesita --talla-cm

Ejemplo (caso tipo 1 de la validación alfa):
  calc-dosis calcular --enfermedad oma --edad-anios 1 --peso-kg 10 --talla-cm 75
)";

/// Error de uso de la línea de órdenes.
class UsageError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

/// Opciones de la línea de órdenes: "--clave valor" o "--bandera".
class Arguments {
 public:
  Arguments(int argc, char** argv, const std::vector<std::string>& flags) {
    for (int i = 2; i < argc; ++i) {
      const std::string key = argv[i];
      if (key.rfind("--", 0) != 0) {
        throw UsageError("Argumento inesperado: " + key);
      }
      const bool is_flag = std::find(flags.begin(), flags.end(), key) != flags.end();
      if (is_flag) {
        values_[key] = "";
      } else if (i + 1 < argc) {
        values_[key] = argv[++i];
      } else {
        throw UsageError("Falta el valor de " + key);
      }
    }
  }

  [[nodiscard]] bool has(const std::string& key) const { return values_.count(key) > 0; }

  [[nodiscard]] std::string text(const std::string& key, const std::string& fallback) const {
    const auto it = values_.find(key);
    return it == values_.end() ? fallback : it->second;
  }

  [[nodiscard]] std::string requiredText(const std::string& key) const {
    if (!has(key)) {
      throw UsageError("Falta la opción obligatoria " + key);
    }
    return values_.at(key);
  }

  /// Número con punto o coma decimal.
  [[nodiscard]] std::optional<double> number(const std::string& key) const {
    if (!has(key)) {
      return std::nullopt;
    }
    std::string raw = values_.at(key);
    for (char& character : raw) {
      if (character == ',') {
        character = '.';
      }
    }
    std::istringstream stream(raw);
    stream.imbue(std::locale::classic());
    double value = 0.0;
    stream >> value;
    if (stream.fail() || !stream.eof()) {
      throw UsageError("El valor de " + key + " no es un número: " + values_.at(key));
    }
    return value;
  }

  [[nodiscard]] double requiredNumber(const std::string& key) const {
    if (const auto value = number(key)) {
      return *value;
    }
    throw UsageError("Falta la opción obligatoria " + key);
  }

 private:
  std::map<std::string, std::string> values_;
};

KnowledgeBase loadKb(const Arguments& arguments) {
  return loadKnowledgeBaseFromFile(arguments.text("--kb", kDefaultKbPath));
}

/// Sexo a partir de --sexo: "m", "f", "masculino" o "femenino".
Sex parseSex(const std::string& text) {
  if (text == "m" || text == "M" || text == "masculino") {
    return Sex::Male;
  }
  if (text == "f" || text == "F" || text == "femenino") {
    return Sex::Female;
  }
  throw UsageError("El valor de --sexo debe ser m o f: " + text);
}

// --- Órdenes -----------------------------------------------------------------

int runValidate(const Arguments& arguments) {
  const KnowledgeBase kb = loadKb(arguments);

  std::size_t regimens = 0;
  std::size_t unverified = 0;
  for (const Disease& disease : kb.diseases) {
    for (const TreatmentLine& line : disease.lines) {
      for (const TherapyOption& option : line.options) {
        for (const Regimen& regimen : option.components) {
          ++regimens;
          if (regimen.status != ReviewStatus::Verified) {
            ++unverified;
          }
        }
      }
    }
  }

  std::cout << "Base de conocimiento válida: versión " << kb.version << " (" << kb.date << ")\n"
            << "  " << kb.diseases.size() << " enfermedades, " << kb.drugs.size()
            << " medicamentos, " << kb.sources.size() << " fuentes, " << regimens
            << " esquemas\n";
  if (unverified > 0) {
    std::cout << "  [!] " << unverified << " de " << regimens
              << " esquemas aún NO están verificados clínicamente.\n";
  }
  return 0;
}

int runList(const Arguments& arguments) {
  const KnowledgeBase kb = loadKb(arguments);
  for (const Disease& disease : kb.diseases) {
    std::cout << disease.id << "\t" << disease.name
              << (disease.antibiotic_indicated ? "" : " (sin antibiótico)") << "\n";
  }
  return 0;
}

int runCalculate(const Arguments& arguments) {
  const KnowledgeBase kb = loadKb(arguments);

  // Edad: en meses o en años cumplidos, pero no ambas.
  if (arguments.has("--edad-meses") == arguments.has("--edad-anios")) {
    throw UsageError("Indique la edad con --edad-meses o con --edad-anios (solo una)");
  }
  const double age_value = arguments.has("--edad-meses") ? arguments.requiredNumber("--edad-meses")
                                                         : arguments.requiredNumber("--edad-anios") * 12.0;
  if (age_value < 0.0 || age_value != static_cast<int>(age_value)) {
    throw UsageError("La edad debe ser un número entero no negativo");
  }

  Patient patient{Age::fromMonths(static_cast<int>(age_value)),
                  Kilograms(arguments.requiredNumber("--peso-kg")), std::nullopt};
  if (const auto height = arguments.number("--talla-cm")) {
    patient.height = Centimeters(*height);
  }
  if (arguments.has("--sexo")) {
    patient.sex = parseSex(arguments.requiredText("--sexo"));
  }

  // Función renal: digitada, estimada o no evaluada.
  if (arguments.has("--tfg") && arguments.has("--creatinina")) {
    throw UsageError("Indique --tfg o --creatinina, no ambas");
  }
  if (arguments.has("--formula") && !arguments.has("--creatinina")) {
    throw UsageError("--formula solo aplica cuando la TFG se estima con --creatinina");
  }
  RenalFunction renal = RenalFunction::assumedNormal();
  if (const auto gfr = arguments.number("--tfg")) {
    renal = RenalFunction::entered(Gfr(*gfr));
  } else if (const auto creatinine = arguments.number("--creatinina")) {
    const std::string formula_id =
        arguments.text("--formula", std::string(id(kDefaultGfrFormula)));
    const std::optional<GfrFormula> formula = gfrFormulaFromId(formula_id);
    if (!formula) {
      throw UsageError("Fórmula desconocida: " + formula_id + " (use ckid-u25 o schwartz)");
    }
    const std::unique_ptr<GfrEstimator> estimator = makeGfrEstimator(*formula);
    const GfrEstimate estimate = estimator->estimate(patient, MgPerDl(*creatinine));
    for (const Warning& warning : estimate.warnings) {
      std::cout << (warning.severity == Severity::Blocking ? "[X] " : "[!] ") << warning.message
                << "\n";
    }
    if (!estimate.gfr) {
      return 2;
    }
    std::cout << "TFG estimada (" << estimator->name() << "): " << estimate.formula << "\n";
    renal = RenalFunction::estimated(*estimate.gfr, estimator->name());
  }

  const CalculationRequest request{patient, renal, arguments.requiredText("--enfermedad")};
  const TreatmentPlan plan = DoseEngine(kb).plan(request);

  cli::PrintOptions options;
  options.show_trace = !arguments.has("--sin-desglose");
  cli::printPlan(std::cout, request, plan, options);
  return plan.blocked ? 2 : 0;
}

}  // namespace

int main(int argc, char** argv) {
#ifdef _WIN32
  // La consola de Windows no usa UTF-8 por defecto; sin esto las tildes se dañan.
  SetConsoleOutputCP(CP_UTF8);
#endif

  if (argc < 2) {
    std::cerr << kUsage;
    return 1;
  }
  const std::string command = argv[1];

  try {
    const Arguments arguments(argc, argv, {"--sin-desglose"});
    if (command == "validar") {
      return runValidate(arguments);
    }
    if (command == "listar") {
      return runList(arguments);
    }
    if (command == "calcular") {
      return runCalculate(arguments);
    }
    if (command == "lote") {
      const int scenarios = cli::runBatch(loadKb(arguments), arguments.requiredText("--entrada"),
                                          arguments.requiredText("--salida"));
      std::cout << scenarios << " escenarios calculados\n";
      return 0;
    }
    if (command == "version" || command == "--version") {
      std::cout << kAppName << " " << kAppVersion << "\n";
      return 0;
    }
    if (command == "ayuda" || command == "--help" || command == "-h") {
      std::cout << kUsage;
      return 0;
    }
    std::cerr << "Orden desconocida: " << command << "\n\n" << kUsage;
    return 1;
  } catch (const UsageError& error) {
    std::cerr << "Error: " << error.what() << "\n\n" << kUsage;
    return 1;
  } catch (const KnowledgeBaseError& error) {
    std::cerr << error.what() << "\n";
    return 1;
  } catch (const NotFoundError& error) {
    std::cerr << "Error: " << error.what() << "\nUse «calc-dosis listar» para ver las enfermedades.\n";
    return 1;
  } catch (const std::exception& error) {
    std::cerr << "Error inesperado: " << error.what() << "\n";
    return 1;
  }
}
