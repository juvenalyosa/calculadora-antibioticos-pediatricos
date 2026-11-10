/// @file result_page.hpp
/// @brief Paso 4: plan de tratamiento calculado.
#pragma once

#include <QWizardPage>

#include "calc/advisor.hpp"
#include "calc/dose_engine.hpp"
#include "session.hpp"

class QTextBrowser;

namespace calc::gui {

class ResultPage final : public QWizardPage {
  Q_OBJECT

 public:
  ResultPage(const Session& session, const KnowledgeBase& kb, const AdvisorRegistry& advisors,
             QWidget* parent = nullptr);

  /// Calcula el plan con lo que hay en la sesión y lo muestra.
  void initializePage() override;

 private:
  const Session& session_;
  DoseEngine engine_;
  const AdvisorRegistry& advisors_;
  QTextBrowser* browser_ = nullptr;
};

}  // namespace calc::gui
