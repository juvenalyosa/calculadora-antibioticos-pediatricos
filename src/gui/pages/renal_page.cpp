#include "pages/renal_page.hpp"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include "html.hpp"

namespace calc::gui {

namespace {
// Unidades de creatinina que ofrece el selector (índices del QComboBox).
enum CreatinineUnit { kMgPerDl = 0, kMicromolPerL = 1 };
}  // namespace

RenalPage::RenalPage(Session& session, QWidget* parent) : QWizardPage(parent), session_(session) {
  setTitle("Paso 2 de 4 · Función renal");
  setSubTitle("Indique la tasa de filtración glomerular (TFG) para ajustar las dosis que lo requieran.");

  not_evaluated_ = new QRadioButton("No evaluada (calcular sin ajuste renal)", this);
  enter_gfr_ = new QRadioButton("Conozco la TFG:", this);
  estimate_ = new QRadioButton("Estimarla con la creatinina sérica:", this);
  not_evaluated_->setChecked(true);

  gfr_ = new QDoubleSpinBox(this);
  gfr_->setRange(0.0, limits::kMaxPlausibleGfr);
  gfr_->setDecimals(1);
  gfr_->setSuffix(" mL/min/1,73 m²");

  creatinine_ = new QDoubleSpinBox(this);
  creatinine_unit_ = new QComboBox(this);
  creatinine_unit_->addItems({"mg/dL", "µmol/L"});
  creatinine_->setDecimals(2);
  creatinine_->setRange(0.0, limits::kMaxPlausibleCreatinineMgDl);
  creatinine_->setSingleStep(0.1);

  // Selector de fórmula, en el orden del catálogo del núcleo (kGfrFormulas).
  formula_label_ = new QLabel("Fórmula:", this);
  formula_label_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  formula_ = new QComboBox(this);
  for (const GfrFormula formula : kGfrFormulas) {
    QString text = toQString(makeGfrEstimator(formula)->name());
    if (formula == GfrFormula::CkidU25) {
      text += " (según edad y sexo)";
    }
    formula_->addItem(text);
    if (formula == kDefaultGfrFormula) {
      formula_->setCurrentIndex(formula_->count() - 1);
    }
  }

  auto* grid = new QGridLayout;
  grid->addWidget(not_evaluated_, 0, 0, 1, 3);
  grid->addWidget(enter_gfr_, 1, 0);
  grid->addWidget(gfr_, 1, 1);
  grid->addWidget(estimate_, 2, 0);
  grid->addWidget(creatinine_, 2, 1);
  grid->addWidget(creatinine_unit_, 2, 2);
  grid->addWidget(formula_label_, 3, 0);
  grid->addWidget(formula_, 3, 1, 1, 2);
  grid->setColumnStretch(3, 1);

  messages_ = new QLabel(this);
  messages_->setWordWrap(true);
  messages_->setTextFormat(Qt::RichText);

  auto* layout = new QVBoxLayout(this);
  layout->addLayout(grid);
  layout->addSpacing(12);
  layout->addWidget(messages_);
  layout->addStretch();

  for (QRadioButton* option : {not_evaluated_, enter_gfr_, estimate_}) {
    connect(option, &QRadioButton::toggled, this, &RenalPage::refresh);
  }
  connect(gfr_, &QDoubleSpinBox::valueChanged, this, &RenalPage::refresh);
  connect(creatinine_, &QDoubleSpinBox::valueChanged, this, &RenalPage::refresh);
  connect(formula_, &QComboBox::currentIndexChanged, this, &RenalPage::refresh);
  connect(creatinine_unit_, &QComboBox::currentIndexChanged, this, [this] {
    // El valor digitado se conserva; solo cambia cómo se interpreta.
    const bool micromol = creatinine_unit_->currentIndex() == kMicromolPerL;
    creatinine_->setDecimals(micromol ? 0 : 2);
    creatinine_->setSingleStep(micromol ? 5.0 : 0.1);
    creatinine_->setRange(0.0, limits::kMaxPlausibleCreatinineMgDl *
                                   (micromol ? kMicromolPerLPerMgPerDlCreatinine : 1.0));
    refresh();
  });

  refresh();
}

void RenalPage::initializePage() {
  refresh();
}

bool RenalPage::isComplete() const {
  return complete_;
}

void RenalPage::setNotEvaluated() {
  not_evaluated_->setChecked(true);
}

void RenalPage::setGfr(double gfr) {
  enter_gfr_->setChecked(true);
  gfr_->setValue(gfr);
}

void RenalPage::setCreatinineMgDl(double creatinine, GfrFormula formula) {
  estimate_->setChecked(true);
  for (std::size_t i = 0; i < kGfrFormulas.size(); ++i) {
    if (kGfrFormulas[i] == formula) {
      formula_->setCurrentIndex(static_cast<int>(i));
    }
  }
  creatinine_unit_->setCurrentIndex(kMgPerDl);
  creatinine_->setValue(creatinine);
}

GfrFormula RenalPage::selectedFormula() const {
  const int index = formula_->currentIndex();
  if (index < 0 || static_cast<std::size_t>(index) >= kGfrFormulas.size()) {
    return kDefaultGfrFormula;
  }
  return kGfrFormulas[static_cast<std::size_t>(index)];
}

MgPerDl RenalPage::creatinineInMgDl() const {
  if (creatinine_unit_->currentIndex() == kMicromolPerL) {
    return mgPerDlFromMicromolPerL(creatinine_->value());
  }
  return MgPerDl(creatinine_->value());
}

void RenalPage::refresh() {
  // Solo se puede escribir en el campo de la opción elegida.
  gfr_->setEnabled(enter_gfr_->isChecked());
  creatinine_->setEnabled(estimate_->isChecked());
  creatinine_unit_->setEnabled(estimate_->isChecked());
  formula_label_->setEnabled(estimate_->isChecked());
  formula_->setEnabled(estimate_->isChecked());

  QString html;
  session_.renal = RenalFunction::assumedNormal();
  complete_ = true;

  if (enter_gfr_->isChecked()) {
    const Gfr gfr(gfr_->value());
    complete_ = false;
    if (gfr.value() > 0.0) {
      const std::vector<Warning> warnings = validateGfr(gfr);
      html = warningsHtml(warnings, palette());
      if (!hasBlocking(warnings)) {
        session_.renal = RenalFunction::entered(gfr);
        complete_ = true;
      }
    }
  } else if (estimate_->isChecked()) {
    complete_ = false;
    const GfrFormula formula = selectedFormula();
    if (creatinine_->value() > 0.0) {
      const std::unique_ptr<GfrEstimator> estimator = makeGfrEstimator(formula);
      const GfrEstimate estimate = estimator->estimate(session_.patient, creatinineInMgDl());
      html = warningsHtml(estimate.warnings, palette());
      if (estimate.gfr) {
        session_.renal = RenalFunction::estimated(*estimate.gfr, estimator->name());
        html = "<p><b>TFG estimada (" + toQString(estimator->name()).toHtmlEscaped() +
               "):</b> " + toQString(estimate.formula).toHtmlEscaped() + "</p>" + html;
        complete_ = true;
      }
    } else {
      // Avisar antes de que el usuario digite: tendrá que volver al paso 1.
      std::vector<Warning> missing;
      if (!session_.patient.height) {
        missing.push_back({Severity::Caution, "TALLA_REQUERIDA",
                           "Para estimar la TFG registre la talla en el paso anterior."});
      }
      if (formula == GfrFormula::CkidU25 && !session_.patient.sex) {
        missing.push_back({Severity::Caution, "SEXO_REQUERIDO",
                           "La ecuación CKiD U25 necesita el sexo: regístrelo en el paso "
                           "anterior o elija la fórmula de Schwartz."});
      }
      html = warningsHtml(missing, palette());
    }
  }

  messages_->setText(html);
  emit completeChanged();
}

}  // namespace calc::gui
