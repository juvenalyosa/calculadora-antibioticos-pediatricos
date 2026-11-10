#include "screenshot_tour.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QPixmap>

#include "pages/disease_page.hpp"
#include "pages/patient_page.hpp"
#include "pages/renal_page.hpp"
#include "report_dialog.hpp"

namespace calc::gui {

namespace {

/// Deja que Qt termine de dibujar y guarda la imagen del widget.
bool capture(QWidget& widget, const QString& folder, const QString& name) {
  QCoreApplication::processEvents();
  return widget.grab().save(QDir(folder).filePath(name));
}

}  // namespace

bool runScreenshotTour(CalculatorWizard& wizard, ReportSink& sink, const QString& folder) {
  if (!QDir().mkpath(folder)) {
    return false;
  }
  wizard.resize(820, 700);
  wizard.show();
  bool ok = true;

  wizard.patientPage()->setValues(1, 0, 10.0, 75.0);
  ok = capture(wizard, folder, "01_paciente.png") && ok;

  wizard.next();
  wizard.renalPage()->setCreatinineMgDl(0.4);
  ok = capture(wizard, folder, "02_funcion_renal.png") && ok;

  wizard.next();
  wizard.diseasePage()->selectDisease("oma");
  ok = capture(wizard, folder, "03_enfermedad.png") && ok;

  wizard.next();
  ok = capture(wizard, folder, "04_resultado.png") && ok;

  // El diálogo de reporte se muestra sin ejecutarlo (no bloquea el recorrido).
  ReportDialog dialog(wizard.buildReportForCurrentStep(), sink, &wizard);
  dialog.show();
  ok = capture(dialog, folder, "05_reporte_de_error.png") && ok;
  dialog.close();

  return ok;
}

}  // namespace calc::gui
