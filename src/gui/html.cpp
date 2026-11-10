#include "html.hpp"

#include "calc/text.hpp"

namespace calc::gui {

namespace {

/// Texto del núcleo, escapado para insertarlo en HTML.
QString esc(std::string_view text) {
  return toQString(text).toHtmlEscaped();
}

/// Colores por gravedad, elegidos según el fondo de la ventana.
struct Colors {
  QString info;
  QString caution;
  QString blocking;
  QString muted;
};

Colors colorsFor(const QPalette& palette) {
  const bool dark = palette.color(QPalette::Window).lightness() < 128;
  if (dark) {
    return {"#8ab4f8", "#fdd663", "#f28b82", "#9aa0a6"};
  }
  return {"#174ea6", "#8a4b00", "#b3261e", "#5f6368"};
}

QString colorOf(Severity severity, const Colors& colors) {
  switch (severity) {
    case Severity::Info:
      return colors.info;
    case Severity::Caution:
      return colors.caution;
    case Severity::Blocking:
      return colors.blocking;
  }
  return colors.info;
}

QString symbolOf(Severity severity) {
  switch (severity) {
    case Severity::Info:
      return QStringLiteral("ⓘ");
    case Severity::Caution:
      return QStringLiteral("⚠");
    case Severity::Blocking:
      return QStringLiteral("⛔");
  }
  return QString();
}

QString sourceHtml(const RegimenResult& result) {
  QString text;
  if (result.source.title.empty()) {
    text = esc(result.source.id);
  } else {
    text = esc(result.source.organization) + " (" + QString::number(result.source.year) + "). <i>" +
           esc(result.source.title) + "</i>";
  }
  if (!result.source.page.empty()) {
    text += ", " + esc(result.source.page);
  }
  return text + ". Estado del dato: " + esc(label(result.status)) + ".";
}

QString regimenHtml(const RegimenResult& result, const Colors& colors, const QPalette& palette) {
  QString html;
  html += "<p style='margin-top:10px'><b>" + esc(result.drug_name) + "</b>, " +
          esc(label(result.route)) + "</p>";

  if (result.not_recommended) {
    html += "<p style='color:" + colors.blocking +
            "; font-size:large'><b>No recomendado con esta función renal</b></p>";
  } else {
    // La dosis es lo primero que debe leer el usuario: va destacada.
    html += "<p style='font-size:x-large; margin:2px 0'><b>" + esc(describeDose(result)) + "</b></p>";
    const std::string duration = describeDuration(result);
    if (!duration.empty()) {
      html += "<p style='margin:2px 0'>Duración: " + esc(duration) + "</p>";
    }

    if (!result.administrations.empty()) {
      html += "<ul style='margin-top:4px'>";
      for (const Administration& administration : result.administrations) {
        html += "<li>" + esc(administration.presentation) + ": ";
        if (administration.suitable) {
          html += "<b>" + esc(describeAdministration(administration)) + "</b> por dosis";
        } else {
          html += "<span style='color:" + colors.muted + "'>no adecuada para esta dosis</span>";
        }
        html += "</li>";
      }
      html += "</ul>";
    }
  }

  if (result.calculated && !result.note.empty()) {
    html += "<p><b>Nota:</b> " + esc(result.note) + "</p>";
  }

  html += warningsHtml(result.warnings, palette);

  if (!result.trace.empty()) {
    html += "<p style='margin-bottom:0; color:" + colors.muted + "'>Cálculo paso a paso</p><ol style='margin-top:2px'>";
    for (const TraceStep& step : result.trace) {
      html += "<li><b>" + esc(step.title) + ":</b> " + esc(step.detail) + "</li>";
    }
    html += "</ol>";
  }

  html += "<p style='color:" + colors.muted + "; font-size:small'>Fuente: " + sourceHtml(result) + "</p>";
  return html;
}

}  // namespace

QString toQString(std::string_view text) {
  return QString::fromUtf8(text.data(), static_cast<qsizetype>(text.size()));
}

QString warningsHtml(const std::vector<Warning>& warnings, const QPalette& palette) {
  const Colors colors = colorsFor(palette);
  QString html;
  for (const Warning& warning : warnings) {
    html += "<p style='margin:4px 0; color:" + colorOf(warning.severity, colors) + "'>" +
            symbolOf(warning.severity) + " " + esc(warning.message) + "</p>";
  }
  return html;
}

QString planHtml(const CalculationRequest& request, const TreatmentPlan& plan,
                 const std::vector<Annotation>& annotations, const QPalette& palette) {
  const Colors colors = colorsFor(palette);
  QString html;

  html += "<h2 style='margin-bottom:2px'>" + esc(plan.disease_name) + "</h2>";
  QString patient = esc(request.patient.age.describe()) + " · " +
                    esc(formatNumber(request.patient.weight.value())) + " kg";
  if (request.patient.height) {
    patient += " · " + esc(formatNumber(request.patient.height->value(), 1)) + " cm";
  }
  if (request.patient.sex) {
    patient += " · sexo " + esc(label(*request.patient.sex));
  }
  html += "<p style='color:" + colors.muted + "'>Paciente: " + patient +
          "<br>Función renal: " + esc(describeRenal(request.renal)) +
          "<br>Base de conocimiento: versión " + esc(plan.kb_version) + "</p>";

  html += warningsHtml(plan.warnings, palette);

  if (plan.blocked) {
    html += "<p><b>No se calculó ninguna dosis.</b> Vuelva atrás y corrija los datos señalados.</p>";
    return html;
  }
  if (!plan.antibiotic_indicated) {
    html += "<h3>No se indica antibiótico</h3><p>" + esc(plan.message) + "</p>";
    return html;
  }

  for (const LineResult& line : plan.lines) {
    html += "<hr><h3>" + esc(label(line.kind)) + "</h3>";
    for (const OptionResult& option : line.options) {
      html += "<p style='margin-bottom:0'><u>Opción " + QString::number(option.number) + ": " +
              esc(option.description) + "</u></p>";
      for (const RegimenResult& component : option.components) {
        html += regimenHtml(component, colors, palette);
      }
    }
    for (const SkippedOption& skipped : line.skipped) {
      html += "<p style='color:" + colors.muted + "'>Opción " + QString::number(skipped.number) +
              ": " + esc(skipped.description) + " — no aplica a este paciente (" +
              esc(skipped.reason) + ").</p>";
    }
  }

  // Las observaciones de módulos de apoyo (p. ej. IA) van aparte y atribuidas:
  // no forman parte del cálculo validado.
  if (!annotations.empty()) {
    html += "<hr><h3>Observaciones de módulos de apoyo</h3>"
            "<p style='color:" + colors.muted +
            "'>No forman parte del cálculo validado.</p>";
    for (const Annotation& annotation : annotations) {
      html += "<p style='color:" + colorOf(annotation.severity, colors) + "'>[" +
              esc(annotation.advisor_id) + "] " + esc(annotation.message) + "</p>";
    }
  }

  html += "<hr><p style='color:" + colors.muted +
          "; font-size:small'>Herramienta de apoyo académico. No reemplaza el juicio clínico ni "
          "la verificación de la dosis antes de formular.</p>";
  return html;
}

}  // namespace calc::gui
