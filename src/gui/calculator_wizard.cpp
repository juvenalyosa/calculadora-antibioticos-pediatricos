#include "calculator_wizard.hpp"

#include <QAbstractButton>

#include "calc/version.hpp"
#include "html.hpp"
#include "pages/disease_page.hpp"
#include "pages/patient_page.hpp"
#include "pages/renal_page.hpp"
#include "pages/result_page.hpp"
#include "report_dialog.hpp"

namespace calc::gui {

CalculatorWizard::CalculatorWizard(const KnowledgeBase& kb, ReportSink& report_sink,
                                   const AdvisorRegistry& advisors, QWidget* parent)
    : QWizard(parent), kb_(kb), report_sink_(report_sink) {
  setWindowTitle(toQString(kAppName) + " " + toQString(kAppVersion) + " — base " +
                 toQString(kb.version));
  // Mismo aspecto en Windows, macOS y Linux.
  setWizardStyle(QWizard::ModernStyle);
  setOption(QWizard::NoBackButtonOnStartPage);
  setOption(QWizard::HaveCustomButton1);
  setMinimumSize(780, 640);

  setButtonText(QWizard::BackButton, "< Atrás");
  setButtonText(QWizard::NextButton, "Siguiente >");
  setButtonText(QWizard::CancelButton, "Salir");
  setButtonText(QWizard::FinishButton, "Nuevo cálculo");
  setButtonText(QWizard::CustomButton1, "Reportar un error…");

  patient_page_ = new PatientPage(session_, this);
  renal_page_ = new RenalPage(session_, this);
  disease_page_ = new DiseasePage(session_, kb, this);
  result_page_ = new ResultPage(session_, kb, advisors, this);
  setPage(kPatientPage, patient_page_);
  setPage(kRenalPage, renal_page_);
  setPage(kDiseasePage, disease_page_);
  setPage(kResultPage, result_page_);

  connect(this, &QWizard::customButtonClicked, this, [this](int which) {
    if (which == QWizard::CustomButton1) {
      openReportDialog();
    }
  });
}

void CalculatorWizard::accept() {
  // No se cierra la ventana: el flujo normal es encadenar varios cálculos.
  restart();
}

std::string CalculatorWizard::currentStepName() const {
  switch (currentId()) {
    case kPatientPage:
      return "paciente";
    case kRenalPage:
      return "funcion_renal";
    case kDiseasePage:
      return "enfermedad";
    case kResultPage:
      return "resultado";
    default:
      return "desconocido";
  }
}

ErrorReport CalculatorWizard::buildReportForCurrentStep() const {
  return makeErrorReport(currentStepName(), ReportCategory::Other, "", session_.request(),
                         kb_.version);
}

void CalculatorWizard::openReportDialog() {
  ReportDialog dialog(buildReportForCurrentStep(), report_sink_, this);
  dialog.exec();
}

}  // namespace calc::gui
