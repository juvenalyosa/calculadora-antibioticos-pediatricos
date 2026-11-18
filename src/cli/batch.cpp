#include "batch.hpp"

#include <fstream>
#include <iomanip>
#include <locale>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "calc/dose_engine.hpp"
#include "calc/renal.hpp"

namespace calc::cli {

namespace {

using Row = std::map<std::string, std::string>;

std::vector<std::string> split(const std::string& line) {
  std::vector<std::string> fields;
  std::string field;
  std::istringstream stream(line);
  while (std::getline(stream, field, ',')) {
    fields.push_back(field);
  }
  if (!line.empty() && line.back() == ',') {
    fields.emplace_back();
  }
  return fields;
}

std::string cell(const Row& row, const std::string& column) {
  const auto it = row.find(column);
  return it == row.end() ? std::string() : it->second;
}

std::optional<double> number(const Row& row, const std::string& column) {
  const std::string text = cell(row, column);
  if (text.empty()) {
    return std::nullopt;
  }
  std::istringstream stream(text);
  stream.imbue(std::locale::classic());
  double value = 0.0;
  stream >> value;
  if (stream.fail()) {
    throw std::runtime_error("valor no numérico en «" + column + "»: " + text);
  }
  return value;
}

/// Identificador estable de la línea, igual al de las hojas de datos.
const char* lineId(TreatmentLineKind kind) {
  switch (kind) {
    case TreatmentLineKind::First:
      return "primera";
    case TreatmentLineKind::Second:
      return "segunda";
    case TreatmentLineKind::Allergy:
      return "alergia";
  }
  return "";
}

const char* routeId(Route route) {
  switch (route) {
    case Route::Oral:
      return "oral";
    case Route::Intravenous:
      return "iv";
    case Route::Intramuscular:
      return "im";
    case Route::Topical:
      return "topica";
  }
  return "";
}

std::string codes(const std::vector<Warning>& warnings) {
  std::string text;
  for (const Warning& warning : warnings) {
    text += (text.empty() ? "" : "|") + warning.code;
  }
  return text;
}

/// Número con punto decimal y sin notación científica.
std::string num(double value) {
  std::ostringstream stream;
  stream.imbue(std::locale::classic());
  stream << std::setprecision(12) << value;
  return stream.str();
}

std::string num(const std::optional<double>& value) {
  return value ? num(*value) : std::string();
}

}  // namespace

int runBatch(const KnowledgeBase& kb, const std::string& input_path,
             const std::string& output_path) {
  std::ifstream input(input_path);
  if (!input.good()) {
    throw std::runtime_error("no se pudo abrir " + input_path);
  }
  std::ofstream output(output_path);
  if (!output.good()) {
    throw std::runtime_error("no se pudo escribir " + output_path);
  }

  std::string line;
  std::getline(input, line);
  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }
  const std::vector<std::string> header = split(line);
  for (const char* required : {"caso", "edad_meses", "peso_kg"}) {
    bool found = false;
    for (const std::string& column : header) {
      found = found || column == required;
    }
    if (!found) {
      throw std::runtime_error(std::string("falta la columna «") + required + "»");
    }
  }

  output << "caso,enfermedad_id,estado,linea,opcion,medicamento_id,via,unidad,dosis_por_toma,"
            "intervalo_horas,dosis_diaria,rango_min,rango_max,tope_aplicado,ajuste_renal_aplicado,"
            "tfg,avisos,presentaciones\n";

  const DoseEngine engine(kb);
  int scenarios = 0;
  while (std::getline(input, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (line.empty()) {
      continue;
    }
    const std::vector<std::string> fields = split(line);
    Row row;
    for (std::size_t i = 0; i < header.size(); ++i) {
      row[header[i]] = i < fields.size() ? fields[i] : std::string();
    }
    const std::string case_id = cell(row, "caso");

    Patient patient{Age::fromMonths(static_cast<int>(number(row, "edad_meses").value_or(0.0))),
                    Kilograms(number(row, "peso_kg").value_or(0.0)), std::nullopt};
    if (const auto height = number(row, "talla_cm")) {
      patient.height = Centimeters(*height);
    }
    const std::string sex = cell(row, "sexo");
    if (sex == "m") {
      patient.sex = Sex::Male;
    } else if (sex == "f") {
      patient.sex = Sex::Female;
    }

    // Función renal: digitada, estimada con la creatinina o no evaluada.
    RenalFunction renal = RenalFunction::assumedNormal();
    std::string renal_codes;
    if (const auto gfr = number(row, "tfg")) {
      renal = RenalFunction::entered(Gfr(*gfr));
    } else if (const auto creatinine = number(row, "creatinina_mg_dl")) {
      const std::string formula_id = cell(row, "formula");
      const GfrFormula formula = gfrFormulaFromId(formula_id).value_or(kDefaultGfrFormula);
      const std::unique_ptr<GfrEstimator> estimator = makeGfrEstimator(formula);
      const GfrEstimate estimate = estimator->estimate(patient, MgPerDl(*creatinine));
      renal_codes = codes(estimate.warnings);
      if (!estimate.gfr) {
        output << case_id << ",,tfg_no_estimable,,,,,,,,,,,,,," << renal_codes << ",\n";
        ++scenarios;
        continue;
      }
      renal = RenalFunction::estimated(*estimate.gfr, estimator->name());
    }
    const std::string gfr_text = renal.gfr ? num(renal.gfr->value()) : std::string();

    std::vector<std::string> diseases;
    if (const std::string requested = cell(row, "enfermedad_id"); !requested.empty()) {
      diseases.push_back(requested);
    } else {
      for (const Disease& disease : kb.diseases) {
        diseases.push_back(disease.id);
      }
    }

    for (const std::string& disease_id : diseases) {
      const TreatmentPlan plan = engine.plan({patient, renal, disease_id});
      const std::string plan_codes =
          codes(plan.warnings) + (renal_codes.empty() ? "" : "|" + renal_codes);
      if (plan.blocked) {
        output << case_id << ',' << disease_id << ",bloqueado,,,,,,,,,,,,," << gfr_text << ','
               << plan_codes << ",\n";
        continue;
      }
      if (!plan.antibiotic_indicated) {
        output << case_id << ',' << disease_id << ",sin_antibiotico,,,,,,,,,,,,," << gfr_text << ','
               << plan_codes << ",\n";
        continue;
      }
      for (const LineResult& line_result : plan.lines) {
        for (const SkippedOption& skipped : line_result.skipped) {
          output << case_id << ',' << disease_id << ",no_aplica," << lineId(line_result.kind) << ','
                 << skipped.number << ",,,,,,,,,,," << gfr_text << ",,\n";
        }
        for (const OptionResult& option : line_result.options) {
          for (const RegimenResult& result : option.components) {
            const char* state = !result.calculated     ? "sin_calculo"
                                : result.not_recommended ? "no_recomendado"
                                                         : "calculado";
            std::string presentations;
            for (const Administration& a : result.administrations) {
              presentations += (presentations.empty() ? "" : "|") + a.presentation_id + '=' +
                               num(a.rounded_quantity) + '=' + num(a.deviation) + '=' +
                               (a.suitable ? "si" : "no");
            }
            output << case_id << ',' << disease_id << ',' << state << ','
                   << lineId(line_result.kind) << ',' << option.number << ',' << result.drug_id
                   << ',' << routeId(result.route) << ',' << symbol(result.unit) << ','
                   << (result.calculated ? num(result.dose_per_administration) : std::string())
                   << ',' << (result.interval_hours ? std::to_string(*result.interval_hours) : "")
                   << ',' << num(result.daily_dose) << ',' << num(result.range_min_per_dose) << ','
                   << num(result.range_max_per_dose) << ',' << (result.max_cap_applied ? "si" : "no")
                   << ',' << (result.renal_adjustment_applied ? "si" : "no") << ',' << gfr_text
                   << ',' << codes(result.warnings) << ',' << presentations << '\n';
          }
        }
      }
    }
    ++scenarios;
  }
  return scenarios;
}

}  // namespace calc::cli
