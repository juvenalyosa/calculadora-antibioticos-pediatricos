#include "pages/renal_page.hpp"

#include <QButtonGroup>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include "calc/text.hpp"
#include "html.hpp"
#include "theme.hpp"
#include "widgets/ui.hpp"

namespace calc::gui {

namespace {
// Unidades de creatinina que ofrece el selector (índices del QComboBox).
enum CreatinineUnit { kMgPerDl = 0, kMicromolPerL = 1 };
}  // namespace

RenalPage::RenalPage(Session& session, QWidget* parent) : QWizardPage(parent), session_(session) {
  not_evaluated_ = new QRadioButton("No evaluada", this);
  enter_gfr_ = new QRadioButton("Conozco la TFG", this);
  estimate_ = new QRadioButton("Estimarla con la creatinina sérica", this);
  not_evaluated_->setChecked(true);
  // Cada botón vive en su propia tarjeta: sin un grupo no serían excluyentes.
  auto* options = new QButtonGroup(this);
  for (QRadioButton* option : {not_evaluated_, enter_gfr_, estimate_}) {
    options->addButton(option);
  }

  gfr_ = new QDoubleSpinBox(this);
  gfr_->setRange(0.0, limits::kMaxPlausibleGfr);
  gfr_->setDecimals(1);
  gfr_->setSuffix(" mL/min/1,73 m²");
  gfr_->setMinimumWidth(200);

  creatinine_ = new QDoubleSpinBox(this);
  creatinine_unit_ = new QComboBox(this);
  creatinine_unit_->addItems({"mg/dL", "µmol/L"});
  creatinine_->setDecimals(2);
  creatinine_->setRange(0.0, limits::kMaxPlausibleCreatinineMgDl);
  creatinine_->setSingleStep(0.1);
  creatinine_->setMinimumWidth(110);
  creatinine_unit_->setMinimumWidth(90);

  // Selector de fórmula, en el orden del catálogo del núcleo (kGfrFormulas).
  formula_label_ = makeFieldLabel("FÓRMULA", this);
  formula_ = new QComboBox(this);
  formula_->setMinimumWidth(240);
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

  // Una tarjeta por opción: el botón de radio, una línea de ayuda y sus campos.
  const auto option_card = [this](QRadioButton* radio, const QString& hint, QLayout* fields) {
    QFrame* card = makeCard(this, "optionCard");
    auto* box = new QVBoxLayout(card);
    box->setContentsMargins(16, 12, 16, 12);
    box->setSpacing(6);
    box->addWidget(radio);
    QLabel* help = makeHint(hint, card);
    help->setContentsMargins(26, 0, 0, 0);
    box->addWidget(help);
    if (fields != nullptr) {
      fields->setContentsMargins(26, 4, 0, 0);
      box->addLayout(fields);
    }
    return card;
  };

  auto* gfr_fields = new QHBoxLayout;
  gfr_fields->addWidget(gfr_);
  gfr_fields->addStretch();

  auto* estimate_fields = new QGridLayout;
  estimate_fields->setHorizontalSpacing(10);
  estimate_fields->setVerticalSpacing(8);
  estimate_fields->addWidget(makeFieldLabel("CREATININA", this), 0, 0);
  estimate_fields->addWidget(creatinine_, 0, 1);
  estimate_fields->addWidget(creatinine_unit_, 0, 2);
  estimate_fields->addWidget(formula_label_, 1, 0);
  estimate_fields->addWidget(formula_, 1, 1, 1, 2);
  estimate_fields->setColumnStretch(3, 1);

  not_evaluated_card_ = option_card(
      not_evaluated_, "Las dosis se calculan sin ajuste renal y el resultado lo advierte.", nullptr);
  enter_gfr_card_ = option_card(
      enter_gfr_, "Digite la tasa de filtración glomerular ya calculada.", gfr_fields);
  estimate_card_ = option_card(
      estimate_, "Usa la talla (y el sexo, según la fórmula) registrados en el paso anterior.",
      estimate_fields);

  messages_ = new QLabel(this);
  messages_->setObjectName("messages");
  messages_->setWordWrap(true);
  messages_->setTextFormat(Qt::RichText);

  auto* layout = new QVBoxLayout(this);
  layout->setSpacing(10);
  layout->addWidget(makePageHeader(
      1, "Función renal",
      "Indique la tasa de filtración glomerular (TFG) para ajustar las dosis que lo requieran.",
      this));
  layout->addWidget(not_evaluated_card_);
  layout->addWidget(enter_gfr_card_);
  layout->addWidget(estimate_card_);
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

  // La tarjeta de la opción elegida se resalta.
  setStyleProperty(not_evaluated_card_, "selected", not_evaluated_->isChecked());
  setStyleProperty(enter_gfr_card_, "selected", enter_gfr_->isChecked());
  setStyleProperty(estimate_card_, "selected", estimate_->isChecked());

  QString html;
  session_.renal = RenalFunction::assumedNormal();
  complete_ = true;

  if (enter_gfr_->isChecked()) {
    const Gfr gfr(gfr_->value());
    complete_ = false;
    if (gfr.value() > 0.0) {
      const std::vector<Warning> warnings = validateGfr(gfr);
      html = warningsHtml(warnings);
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
      html = warningsHtml(estimate.warnings);
      if (estimate.gfr) {
        session_.renal = RenalFunction::estimated(*estimate.gfr, estimator->name());
        // El valor estimado va destacado; debajo, la cuenta para rehacerla a mano.
        const Theme& t = theme();
        html = "<table width='100%' cellspacing='0' cellpadding='0'><tr><td style='padding:6px 4px'>"
               "<p style='margin:0; font-size:11px; font-weight:600; color:" + t.muted +
               "'>TFG ESTIMADA · " + toQString(estimator->name()).toUpper().toHtmlEscaped() +
               "</p><p style='margin:2px 0 0 0; font-size:22px; font-weight:600; color:" +
               t.primary + "'>" + toQString(formatNumber(estimate.gfr->value(), 1)) +
               " mL/min/1,73 m²</p><p style='margin:4px 0 0 0; color:" + t.muted + "'>" +
               toQString(estimate.formula).toHtmlEscaped() + "</p></td></tr></table>" + html;
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
      html = warningsHtml(missing);
    }
  }

  messages_->setText(html);
  messages_->setVisible(!messages_->text().isEmpty());
  emit completeChanged();
}

}  // namespace calc::gui
