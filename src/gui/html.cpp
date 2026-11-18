#include "html.hpp"

#include "calc/text.hpp"
#include "theme.hpp"

// El HTML se muestra en QTextBrowser/QLabel, que entienden solo un subconjunto
// de HTML 4 y CSS: no hay esquinas redondeadas ni sombras. Por eso las tarjetas
// y los avisos se arman con tablas de una fila, con color de fondo y borde.

namespace calc::gui {

namespace {

/// Texto del núcleo, escapado para insertarlo en HTML.
QString esc(std::string_view text) {
  return toQString(text).toHtmlEscaped();
}

struct Tone {
  QString text, background, accent, symbol;
};

Tone toneOf(Severity severity) {
  const Theme& t = theme();
  switch (severity) {
    case Severity::Info:
      return {t.info_text, t.info_bg, t.info_accent, QStringLiteral("ⓘ")};
    case Severity::Caution:
      return {t.caution_text, t.caution_bg, t.caution_accent, QStringLiteral("⚠")};
    case Severity::Blocking:
      return {t.danger_text, t.danger_bg, t.danger_accent, QStringLiteral("⛔")};
  }
  return {t.info_text, t.info_bg, t.info_accent, QString()};
}

/// Aviso: franja de color a la izquierda y texto sobre fondo suave.
QString callout(const Tone& tone, const QString& body_html) {
  return "<table width='100%' cellspacing='0' cellpadding='0' style='margin-top:6px'><tr>"
         "<td width='4' bgcolor='" + tone.accent + "'></td>"
         "<td bgcolor='" + tone.background + "' style='padding:7px 10px; color:" + tone.text +
         "'>" + body_html + "</td></tr></table>";
}

/// Etiqueta pequeña con fondo de color (estado del dato, vía…).
QString pill(const QString& text, const QString& foreground, const QString& background) {
  return "<span style='font-size:11px; font-weight:600; color:" + foreground +
         "; background-color:" + background + "'>&nbsp;" + text.toHtmlEscaped() +
         "&nbsp;</span>";
}

QString statusPill(ReviewStatus status) {
  const Theme& t = theme();
  const QString text = toQString(label(status)).toUpper();
  if (status == ReviewStatus::Verified) {
    return pill(text, t.success_text, t.success_bg);
  }
  return pill(text + " · SIN VERIFICAR", t.caution_text, t.caution_bg);
}

QString lineColor(TreatmentLineKind kind) {
  const Theme& t = theme();
  switch (kind) {
    case TreatmentLineKind::First:
      return t.line_first;
    case TreatmentLineKind::Second:
      return t.line_second;
    case TreatmentLineKind::Allergy:
      return t.line_allergy;
  }
  return t.line_first;
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
  return text + ".";
}

/// Presentaciones: una fila por presentación, con la cantidad a la derecha.
QString administrationsHtml(const RegimenResult& result) {
  const Theme& t = theme();
  if (result.administrations.empty()) {
    return QString();
  }
  QString html = "<table width='100%' cellspacing='0' cellpadding='0' style='margin-top:8px'>";
  for (const Administration& administration : result.administrations) {
    html += "<tr><td style='padding:5px 0; border-top:1px solid " + t.border + "; color:" +
            (administration.suitable ? t.text : t.muted) + "'>" +
            esc(administration.presentation) + "</td>"
            "<td align='right' style='padding:5px 0; border-top:1px solid " + t.border + "'>";
    if (administration.suitable) {
      html += "<b>" + esc(describeAdministration(administration)) + "</b> por dosis";
    } else {
      html += "<span style='color:" + t.muted + "'>no adecuada para esta dosis</span>";
    }
    html += "</td></tr>";
  }
  return html + "</table>";
}

/// Un medicamento de una opción: dosis destacada, presentaciones, avisos,
/// desglose y fuente.
QString regimenHtml(const RegimenResult& result) {
  const Theme& t = theme();
  QString html;

  html += "<p style='margin:0'><span style='font-size:15px; font-weight:600'>" +
          esc(result.drug_name) + "</span> <span style='color:" + t.muted + "'>· " +
          esc(label(result.route)) + "</span></p>";

  if (result.not_recommended) {
    html += "<p style='margin:4px 0 0 0; font-size:18px; font-weight:600; color:" + t.danger_text +
            "'>No recomendado con esta función renal</p>";
  } else {
    // La dosis es lo primero que debe leer el usuario: va destacada.
    html += "<p style='margin:4px 0 0 0; font-size:26px; font-weight:600; color:" + t.primary +
            "'>" + esc(describeDose(result)) + "</p>";
    const std::string duration = describeDuration(result);
    if (!duration.empty()) {
      html += "<p style='margin:2px 0 0 0; color:" + t.muted + "'>Duración: <span style='color:" +
              t.text + "'>" + esc(duration) + "</span></p>";
    }
    html += administrationsHtml(result);
  }

  if (result.calculated && !result.note.empty()) {
    html += "<p style='margin:10px 0 0 0'><b>Nota.</b> " + esc(result.note) + "</p>";
  }

  // El estado «sin verificar» ya está en la etiqueta de la opción y en el aviso
  // general del resultado: aquí no se repite en cada medicamento.
  std::vector<Warning> shown;
  for (const Warning& warning : result.warnings) {
    if (warning.code != "DATOS_NO_VERIFICADOS") {
      shown.push_back(warning);
    }
  }
  html += warningsHtml(shown);

  if (!result.trace.empty()) {
    html += "<table width='100%' cellspacing='0' cellpadding='0' style='margin-top:10px'><tr>"
            "<td bgcolor='" + t.surface_alt + "' style='padding:8px 12px'>"
            "<p style='margin:0; font-size:11px; font-weight:600; color:" + t.muted +
            "'>CÁLCULO PASO A PASO</p>"
            "<ol style='margin:4px 0 0 0'>";
    for (const TraceStep& step : result.trace) {
      html += "<li style='margin-top:2px'><b>" + esc(step.title) + ":</b> " + esc(step.detail) +
              "</li>";
    }
    html += "</ol></td></tr></table>";
  }

  html += "<p style='margin:8px 0 0 0; font-size:12px; color:" + t.muted + "'>Fuente: " +
          sourceHtml(result) + "</p>";
  return html;
}

/// Tarjeta de una opción de tratamiento, con todos sus medicamentos.
QString optionHtml(const OptionResult& option, const QString& accent) {
  const Theme& t = theme();
  QString html =
      "<table width='100%' cellspacing='0' cellpadding='0' style='margin-top:10px; "
      "border:1px solid " + t.border + "'><tr>"
      "<td width='5' bgcolor='" + accent + "'></td>"
      "<td bgcolor='" + t.surface + "' style='padding:12px 16px'>";

  QString status;
  if (!option.components.empty()) {
    status = statusPill(option.components.front().status);
  }
  html += "<table width='100%' cellspacing='0' cellpadding='0'><tr>"
          "<td style='font-size:12px; font-weight:600; color:" + t.muted + "'>OPCIÓN " +
          QString::number(option.number) + " · " + esc(option.description) + "</td>"
          "<td align='right'>" + status + "</td></tr></table>"
          "<p style='margin:0; font-size:6px'>&nbsp;</p>";

  bool first = true;
  for (const RegimenResult& component : option.components) {
    if (!first) {
      // Terapia combinada: los medicamentos se dan juntos.
      html += "<p style='margin:10px 0; color:" + t.muted + "; font-weight:600'>+ junto con</p>";
    }
    html += regimenHtml(component);
    first = false;
  }
  return html + "</td></tr></table>";
}

/// Tarjeta de resumen: enfermedad y datos del cálculo.
QString summaryHtml(const CalculationRequest& request, const TreatmentPlan& plan) {
  const Theme& t = theme();
  QString patient = esc(request.patient.age.describe()) + " · " +
                    esc(formatNumber(request.patient.weight.value())) + " kg";
  if (request.patient.height) {
    patient += " · " + esc(formatNumber(request.patient.height->value(), 1)) + " cm";
  }
  if (request.patient.sex) {
    patient += " · sexo " + esc(label(*request.patient.sex));
  }

  const auto cell = [&t](const QString& title, const QString& value) {
    return "<td style='padding:0 18px 0 0'><p style='margin:0; font-size:11px; font-weight:600; "
           "color:" + t.muted + "'>" + title + "</p><p style='margin:2px 0 0 0'>" + value +
           "</p></td>";
  };

  return "<table width='100%' cellspacing='0' cellpadding='0' style='border:1px solid " +
         t.border + "'><tr><td bgcolor='" + t.window + "' style='padding:14px 16px'>"
         "<p style='margin:0; font-size:20px; font-weight:600'>" + esc(plan.disease_name) + "</p>"
         "<table cellspacing='0' cellpadding='0' style='margin-top:10px'><tr>" +
         cell("PACIENTE", patient) + cell("FUNCIÓN RENAL", esc(describeRenal(request.renal))) +
         cell("BASE DE CONOCIMIENTO", "versión " + esc(plan.kb_version)) +
         "</tr></table></td></tr></table>";
}

}  // namespace

QString toQString(std::string_view text) {
  return QString::fromUtf8(text.data(), static_cast<qsizetype>(text.size()));
}

QString warningsHtml(const std::vector<Warning>& warnings) {
  QString html;
  for (const Warning& warning : warnings) {
    const Tone tone = toneOf(warning.severity);
    html += callout(tone, tone.symbol + "&nbsp; " + esc(warning.message));
  }
  return html;
}

QString noticeHtml(Severity severity, const QString& body_html) {
  return callout(toneOf(severity), body_html);
}

QString diseasePreviewHtml(const KnowledgeBase& kb, const Disease& disease) {
  const Theme& t = theme();
  QString html = "<div style='color:" + t.text + "; font-size:14px'>"
                 "<p style='margin:0; font-size:18px; font-weight:600'>" + esc(disease.name) + "</p>"
                 "<p style='margin:6px 0 0 0'>" +
                 (disease.antibiotic_indicated
                      ? pill("CON ANTIBIÓTICO", t.info_text, t.info_bg)
                      : pill("SIN ANTIBIÓTICO", t.success_text, t.success_bg)) +
                 "</p>";

  if (!disease.antibiotic_indicated) {
    html += "<p style='margin:12px 0 0 0'>" + esc(disease.message) + "</p>";
  }
  for (const TreatmentLine& line : disease.lines) {
    html += "<p style='margin:14px 0 2px 0; font-size:11px; font-weight:600; color:" +
            lineColor(line.kind) + "'>" + esc(label(line.kind)).toUpper() + "</p>";
    for (const TherapyOption& option : line.options) {
      html += "<p style='margin:2px 0 0 0'>•&nbsp; " + esc(option.description) + "</p>";
    }
  }

  if (const Source* source = kb.findSource(disease.source_id)) {
    html += "<p style='margin:16px 0 2px 0; font-size:11px; font-weight:600; color:" + t.muted +
            "'>GUÍA PRINCIPAL</p><p style='margin:0; font-size:12px; color:" + t.muted + "'>" +
            esc(source->organization) +
            (source->year > 0 ? " (" + QString::number(source->year) + ")" : QString()) +
            ". <i>" + esc(source->title) + "</i>.</p>";
  }
  return html + "</div>";
}

QString planHtml(const CalculationRequest& request, const TreatmentPlan& plan,
                 const std::vector<Annotation>& annotations) {
  const Theme& t = theme();
  QString html = "<div style='color:" + t.text + "; font-size:14px'>";

  html += summaryHtml(request, plan);
  html += warningsHtml(plan.warnings);

  if (plan.blocked) {
    html += noticeHtml(Severity::Blocking,
                       "<b>No se calculó ninguna dosis.</b> Vuelva atrás y corrija los datos "
                       "señalados.");
    return html + "</div>";
  }
  if (!plan.antibiotic_indicated) {
    html += "<table width='100%' cellspacing='0' cellpadding='0' style='margin-top:10px; "
            "border:1px solid " + t.border + "'><tr>"
            "<td width='5' bgcolor='" + t.success_accent + "'></td>"
            "<td bgcolor='" + t.surface + "' style='padding:14px 16px'>"
            "<p style='margin:0; font-size:20px; font-weight:600; color:" + t.success_text +
            "'>No se indica antibiótico</p><p style='margin:6px 0 0 0'>" + esc(plan.message) +
            "</p></td></tr></table>";
    return html + "</div>";
  }

  // Un solo aviso para todos los esquemas sin verificar del resultado.
  bool any_unverified = false;
  for (const LineResult& line : plan.lines) {
    for (const OptionResult& option : line.options) {
      for (const RegimenResult& component : option.components) {
        any_unverified = any_unverified || component.status != ReviewStatus::Verified;
      }
    }
  }
  if (any_unverified) {
    html += noticeHtml(Severity::Caution,
                       "⚠&nbsp; <b>Contenido sin verificar clínicamente.</b> Los esquemas "
                       "marcados «BORRADOR» o «MUESTRA» no han tenido la doble verificación "
                       "contra la guía. No usar para decisiones clínicas.");
  }

  for (const LineResult& line : plan.lines) {
    const QString accent = lineColor(line.kind);
    html += "<p style='margin:20px 0 0 0; font-size:16px; font-weight:600; color:" + accent +
            "'>" + esc(label(line.kind)) + "</p>";
    for (const OptionResult& option : line.options) {
      html += optionHtml(option, accent);
    }
    for (const SkippedOption& skipped : line.skipped) {
      html += "<p style='margin:8px 0 0 0; color:" + t.muted + "'>Opción " +
              QString::number(skipped.number) + ": " + esc(skipped.description) +
              " — no aplica a este paciente (" + esc(skipped.reason) + ").</p>";
    }
  }

  // Las observaciones de módulos de apoyo (p. ej. IA) van aparte y atribuidas:
  // no forman parte del cálculo validado.
  if (!annotations.empty()) {
    html += "<p style='margin:20px 0 0 0; font-size:16px; font-weight:600'>Observaciones de "
            "módulos de apoyo</p><p style='margin:2px 0 0 0; color:" + t.muted +
            "'>No forman parte del cálculo validado.</p>";
    for (const Annotation& annotation : annotations) {
      html += callout(toneOf(annotation.severity),
                      "[" + esc(annotation.advisor_id) + "] " + esc(annotation.message));
    }
  }

  html += "<p style='margin:18px 0 0 0; font-size:12px; color:" + t.muted +
          "'>Herramienta de apoyo académico. No reemplaza el juicio clínico ni la verificación "
          "de la dosis antes de formular.</p>";
  return html + "</div>";
}

}  // namespace calc::gui
