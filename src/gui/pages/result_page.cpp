#include "pages/result_page.hpp"

#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

#include "html.hpp"

namespace calc::gui {

ResultPage::ResultPage(const Session& session, const KnowledgeBase& kb,
                       const AdvisorRegistry& advisors, QWidget* parent)
    : QWizardPage(parent), session_(session), engine_(kb), advisors_(advisors) {
  setTitle("Paso 4 de 4 · Resultado");
  setSubTitle("Verifique la dosis con el cálculo paso a paso antes de usarla.");
  setFinalPage(true);

  browser_ = new QTextBrowser(this);
  browser_->setOpenExternalLinks(false);

  auto* copy = new QPushButton("Copiar resultado", this);
  connect(copy, &QPushButton::clicked, this,
          [this] { QApplication::clipboard()->setText(browser_->toPlainText()); });

  auto* buttons = new QHBoxLayout;
  buttons->addStretch();
  buttons->addWidget(copy);

  auto* layout = new QVBoxLayout(this);
  layout->addWidget(browser_);
  layout->addLayout(buttons);
}

void ResultPage::initializePage() {
  const CalculationRequest request = session_.request();
  // El motor calcula; los módulos de apoyo solo observan el plan terminado.
  const TreatmentPlan plan = engine_.plan(request);
  const std::vector<Annotation> annotations = advisors_.reviewAll({request, plan});
  browser_->setHtml(planHtml(request, plan, annotations, palette()));
}

}  // namespace calc::gui
