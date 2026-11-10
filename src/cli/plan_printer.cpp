#include "plan_printer.hpp"

#include <ostream>
#include <string>

#include "calc/text.hpp"

namespace calc::cli {

namespace {

const char* marker(Severity severity) {
  switch (severity) {
    case Severity::Info:
      return "[i]";
    case Severity::Caution:
      return "[!]";
    case Severity::Blocking:
      return "[X]";
  }
  return "";
}

void printWarnings(std::ostream& out, const std::vector<Warning>& warnings,
                   const std::string& indent) {
  for (const Warning& warning : warnings) {
    out << indent << marker(warning.severity) << " " << warning.message << "\n";
  }
}

void printRegimen(std::ostream& out, const RegimenResult& result, const PrintOptions& options) {
  const std::string indent = "      ";
  out << "    " << result.drug_name << ", " << label(result.route) << "\n";

  if (result.not_recommended) {
    out << indent << "NO RECOMENDADO con esta función renal.\n";
  } else {
    out << indent << "Dosis: " << describeDose(result);
    const std::string duration = describeDuration(result);
    if (!duration.empty()) {
      out << " durante " << duration;
    }
    out << "\n";

    for (const Administration& administration : result.administrations) {
      out << indent << "  - " << administration.presentation << ": ";
      if (administration.suitable) {
        out << describeAdministration(administration) << " por dosis\n";
      } else {
        out << "no adecuada para esta dosis\n";
      }
    }
  }

  if (result.calculated && !result.note.empty()) {
    out << indent << "Nota: " << result.note << "\n";
  }

  if (options.show_trace && !result.trace.empty()) {
    out << indent << "Cálculo:\n";
    int step_number = 1;
    for (const TraceStep& step : result.trace) {
      out << indent << "  " << step_number++ << ". " << step.title << ": " << step.detail << "\n";
    }
  }

  out << indent << "Fuente: ";
  if (result.source.title.empty()) {
    out << result.source.id;
  } else {
    out << result.source.organization << " (" << result.source.year << "). "
        << result.source.title;
  }
  if (!result.source.page.empty()) {
    out << ", " << result.source.page;
  }
  out << ". Estado: " << label(result.status) << ".\n";

  printWarnings(out, result.warnings, indent);
}

}  // namespace

void printPlan(std::ostream& out, const CalculationRequest& request, const TreatmentPlan& plan,
               const PrintOptions& options) {
  out << "Paciente: " << request.patient.age.describe() << ", "
      << formatNumber(request.patient.weight.value()) << " kg";
  if (request.patient.height) {
    out << ", " << formatNumber(request.patient.height->value(), 1) << " cm";
  }
  if (request.patient.sex) {
    out << ", sexo " << label(*request.patient.sex);
  }
  out << "\n";
  out << "Función renal: " << describeRenal(request.renal) << "\n";
  out << "Enfermedad: " << plan.disease_name << "\n";
  out << "Base de conocimiento: versión " << plan.kb_version << "\n";

  if (!plan.warnings.empty()) {
    out << "\n";
    printWarnings(out, plan.warnings, "");
  }

  if (plan.blocked) {
    out << "\nNo se calculó ninguna dosis: corrija los datos señalados con [X].\n";
    return;
  }
  if (!plan.antibiotic_indicated) {
    out << "\nNO SE INDICA ANTIBIÓTICO. " << plan.message << "\n";
    return;
  }

  for (const LineResult& line : plan.lines) {
    out << "\n=== " << label(line.kind) << " ===\n";
    for (const OptionResult& option : line.options) {
      out << "  Opción " << option.number << ": " << option.description << "\n";
      for (const RegimenResult& component : option.components) {
        printRegimen(out, component, options);
      }
    }
    for (const SkippedOption& skipped : line.skipped) {
      out << "  Opción " << skipped.number << ": " << skipped.description
          << " — no aplica a este paciente (" << skipped.reason << ")\n";
    }
  }
}

}  // namespace calc::cli
