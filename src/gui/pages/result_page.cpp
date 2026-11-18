#include "pages/result_page.hpp"

#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

#include "html.hpp"
#include "widgets/ui.hpp"

namespace calc::gui {

ResultPage::ResultPage(const Session& session, const KnowledgeBase& kb,
                       const AdvisorRegistry& advisors, QWidget* parent)
    : QWizardPage(parent), session_(session), engine_(kb), advisors_(advisors) {
  setFinalPage(true);

  browser_ = new QTextBrowser(this);
  browser_->setOpenExternalLinks(false);
  browser_->setFrameShape(QFrame::NoFrame);

  auto* copy = new QPushButton("Copiar resultado", this);
  connect(copy, &QPushButton::clicked, this,
          [this] { QApplication::clipboard()->setText(browser_->toPlainText()); });

  auto* actions = new QHBoxLayout;
  actions->addStretch();
  actions->addWidget(copy);

  auto* layout = new QVBoxLayout(this);
  layout->setSpacing(10);
  layout->addWidget(makePageHeader(
      3, "Resultado", "Verifique la dosis con el cálculo paso a paso antes de usarla.", this));
  layout->addWidget(browser_, 1);
  layout->addLayout(actions);
}

void ResultPage::initializePage() {
  const CalculationRequest request = session_.request();
  // El motor calcula; los módulos de apoyo solo observan el plan terminado.
  const TreatmentPlan plan = engine_.plan(request);
  const std::vector<Annotation> annotations = advisors_.reviewAll({request, plan});
  browser_->setHtml(planHtml(request, plan, annotations));
}

}  // namespace calc::gui
