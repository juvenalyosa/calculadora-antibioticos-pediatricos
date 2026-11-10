/// @file renal_page.hpp
/// @brief Paso 2: función renal (no evaluada, TFG digitada o estimada).
#pragma once

#include <QWizardPage>

#include "calc/renal.hpp"
#include "session.hpp"

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QRadioButton;

namespace calc::gui {

class RenalPage final : public QWizardPage {
  Q_OBJECT

 public:
  explicit RenalPage(Session& session, QWidget* parent = nullptr);

  /// Al entrar se recalcula: la talla del paso 1 puede haber cambiado.
  void initializePage() override;
  [[nodiscard]] bool isComplete() const override;

  // Carga valores sin intervención del usuario (pruebas y capturas).
  void setNotEvaluated();
  void setGfr(double gfr);
  void setCreatinineMgDl(double creatinine);

 private:
  /// Creatinina digitada, convertida a mg/dL según la unidad elegida.
  [[nodiscard]] MgPerDl creatinineInMgDl() const;
  /// Widgets → sesión, validación y mensajes. Se llama en cada cambio.
  void refresh();

  Session& session_;
  BedsideSchwartzEstimator estimator_;

  QRadioButton* not_evaluated_ = nullptr;
  QRadioButton* enter_gfr_ = nullptr;
  QRadioButton* estimate_ = nullptr;
  QDoubleSpinBox* gfr_ = nullptr;
  QDoubleSpinBox* creatinine_ = nullptr;
  QComboBox* creatinine_unit_ = nullptr;
  QLabel* messages_ = nullptr;
  bool complete_ = true;
};

}  // namespace calc::gui
