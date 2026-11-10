#include "calc/error_report.hpp"

#include <ctime>
#include <fstream>
#include <locale>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "calc/version.hpp"

namespace calc {

namespace {

/// Fecha y hora actuales en UTC, formato ISO 8601.
std::string nowUtcIso8601() {
  const std::time_t now = std::time(nullptr);
  std::tm utc{};
#ifdef _WIN32
  gmtime_s(&utc, &now);
#else
  gmtime_r(&now, &utc);
#endif
  char buffer[32];
  std::strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%SZ", &utc);
  return buffer;
}

/// Número en formato neutro (punto decimal) para que la hoja de errores lo
/// pueda leer como dato, independiente de la configuración regional.
std::string plainNumber(double value) {
  std::ostringstream stream;
  stream.imbue(std::locale::classic());
  stream << value;
  return stream.str();
}

std::string_view toString(RenalSource source) noexcept {
  switch (source) {
    case RenalSource::AssumedNormal:
      return "asumida_normal";
    case RenalSource::EnteredDirectly:
      return "digitada";
    case RenalSource::Estimated:
      return "estimada";
  }
  return "";
}

}  // namespace

std::string_view toString(ReportCategory category) noexcept {
  switch (category) {
    case ReportCategory::CalculationError:
      return "error_de_calculo";
    case ReportCategory::ClinicalContent:
      return "contenido_clinico";
    case ReportCategory::Usability:
      return "usabilidad";
    case ReportCategory::Other:
      return "otro";
  }
  return "";
}

ErrorReport makeErrorReport(const std::string& step, ReportCategory category,
                            const std::string& comment, const CalculationRequest& request,
                            const std::string& kb_version) {
  ErrorReport report;
  report.timestamp_utc = nowUtcIso8601();
  report.app_version = std::string(kAppVersion);
  report.kb_version = kb_version;
  report.step = step;
  report.category = category;
  report.comment = comment;

  // Solo variables del cálculo: nada que identifique al paciente.
  report.context["edad_meses"] = std::to_string(request.patient.age.totalMonths());
  report.context["peso_kg"] = plainNumber(request.patient.weight.value());
  if (request.patient.height) {
    report.context["talla_cm"] = plainNumber(request.patient.height->value());
  }
  report.context["funcion_renal"] = std::string(toString(request.renal.source));
  if (request.renal.gfr) {
    report.context["tfg"] = plainNumber(request.renal.gfr->value());
  }
  if (!request.renal.method.empty()) {
    report.context["metodo_tfg"] = request.renal.method;
  }
  if (!request.disease_id.empty()) {
    report.context["enfermedad_id"] = request.disease_id;
  }
  return report;
}

void JsonlFileSink::submit(const ErrorReport& report) {
  nlohmann::json line = {
      {"fecha_utc", report.timestamp_utc},
      {"version_app", report.app_version},
      {"version_base", report.kb_version},
      {"paso", report.step},
      {"categoria", std::string(toString(report.category))},
      {"comentario", report.comment},
      {"parametros", report.context},
  };

  std::ofstream file(path_, std::ios::app);
  if (!file) {
    throw std::runtime_error("No se pudo abrir el archivo de reportes: " + path_.string());
  }
  file << line.dump() << '\n';
  if (!file) {
    throw std::runtime_error("No se pudo escribir el reporte en: " + path_.string());
  }
}

}  // namespace calc
