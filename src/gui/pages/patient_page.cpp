#include "pages/patient_page.hpp"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

#include "calc/text.hpp"
#include "html.hpp"

namespace calc::gui {

namespace {
// Unidades de peso que ofrece el selector (índices del QComboBox).
enum WeightUnit { kKilograms = 0, kPounds = 1, kGrams = 2 };
// Opciones del selector de sexo (índices del QComboBox).
enum SexChoice { kSexNotRecorded = 0, kSexMale = 1, kSexFemale = 2 };
}  // namespace

PatientPage::PatientPage(Session& session, QWidget* parent)
    : QWizardPage(parent), session_(session) {
  setTitle("Paso 1 de 4 · Paciente");
  setSubTitle(
      "Ingrese la edad y el peso. La talla y el sexo solo se necesitan para estimar la función "
      "renal.");

  // --- Edad ---
  years_ = new QSpinBox(this);
  years_->setRange(0, 17);
  years_->setSuffix(" años");
  months_ = new QSpinBox(this);
  months_->setRange(0, 11);
  months_->setSuffix(" meses");
  auto* age_row = new QHBoxLayout;
  age_row->addWidget(years_);
  age_row->addWidget(months_);
  age_row->addStretch();

  // --- Peso, con selector de unidad ---
  weight_ = new QDoubleSpinBox(this);
  weight_unit_ = new QComboBox(this);
  weight_unit_->addItems({"kg", "lb", "g"});
  weight_in_kg_ = new QLabel(this);
  auto* weight_row = new QHBoxLayout;
  weight_row->addWidget(weight_);
  weight_row->addWidget(weight_unit_);
  weight_row->addWidget(weight_in_kg_);
  weight_row->addStretch();
  applyWeightUnit();

  // --- Talla (opcional) ---
  height_ = new QDoubleSpinBox(this);
  height_->setRange(0.0, limits::kMaxHeightCm);
  height_->setDecimals(1);
  height_->setSuffix(" cm");
  height_->setSpecialValueText("No registrada");  // se muestra cuando vale 0
  auto* height_row = new QHBoxLayout;
  height_row->addWidget(height_);
  height_row->addStretch();

  // --- Sexo (opcional; lo usa la ecuación CKiD U25) ---
  sex_ = new QComboBox(this);
  sex_->addItems({"No registrado", "Masculino", "Femenino"});
  auto* sex_row = new QHBoxLayout;
  sex_row->addWidget(sex_);
  sex_row->addStretch();

  auto* form = new QFormLayout;
  form->addRow("Edad:", age_row);
  form->addRow("Peso:", weight_row);
  form->addRow("Talla:", height_row);
  form->addRow("Sexo:", sex_row);

  messages_ = new QLabel(this);
  messages_->setWordWrap(true);
  messages_->setTextFormat(Qt::RichText);

  // Aviso permanente en la primera pantalla.
  auto* disclaimer = new QLabel(
      "Herramienta de apoyo académico. No reemplaza el juicio clínico ni la verificación de la "
      "dosis antes de formular. No se guarda ningún dato que identifique al paciente.",
      this);
  disclaimer->setWordWrap(true);

  auto* layout = new QVBoxLayout(this);
  layout->addLayout(form);
  layout->addSpacing(12);
  layout->addWidget(messages_);
  layout->addStretch();
  layout->addWidget(disclaimer);

  connect(years_, &QSpinBox::valueChanged, this, &PatientPage::refresh);
  connect(months_, &QSpinBox::valueChanged, this, &PatientPage::refresh);
  connect(weight_, &QDoubleSpinBox::valueChanged, this, &PatientPage::refresh);
  connect(height_, &QDoubleSpinBox::valueChanged, this, &PatientPage::refresh);
  connect(sex_, &QComboBox::currentIndexChanged, this, &PatientPage::refresh);
  connect(weight_unit_, &QComboBox::currentIndexChanged, this, [this] {
    applyWeightUnit();
    refresh();
  });

  refresh();
}

bool PatientPage::isComplete() const {
  return complete_;
}

void PatientPage::setValues(int years, int months, double weight_kg, double height_cm,
                            std::optional<Sex> sex) {
  years_->setValue(years);
  months_->setValue(months);
  weight_unit_->setCurrentIndex(kKilograms);
  weight_->setValue(weight_kg);
  height_->setValue(height_cm);
  sex_->setCurrentIndex(!sex ? kSexNotRecorded : (*sex == Sex::Male ? kSexMale : kSexFemale));
}

double PatientPage::weightInKilograms() const {
  const double value = weight_->value();
  switch (weight_unit_->currentIndex()) {
    case kPounds:
      return kilogramsFromPounds(value).value();
    case kGrams:
      return kilogramsFromGrams(value).value();
    default:
      return value;
  }
}

void PatientPage::applyWeightUnit() {
  // El valor digitado se conserva; solo cambia cómo se interpreta.
  switch (weight_unit_->currentIndex()) {
    case kPounds:
      weight_->setDecimals(1);
      weight_->setRange(0.0, limits::kMaxWeightKg / kKilogramsPerPound);
      break;
    case kGrams:
      weight_->setDecimals(0);
      weight_->setRange(0.0, limits::kMaxWeightKg * 1000.0);
      break;
    default:
      weight_->setDecimals(2);
      weight_->setRange(0.0, limits::kMaxWeightKg);
      break;
  }
}

void PatientPage::refresh() {
  const double kg = weightInKilograms();

  session_.patient.age = Age::fromYearsAndMonths(years_->value(), months_->value());
  session_.patient.weight = Kilograms(kg);
  if (height_->value() > 0.0) {
    session_.patient.height = Centimeters(height_->value());
  } else {
    session_.patient.height.reset();
  }
  switch (sex_->currentIndex()) {
    case kSexMale:
      session_.patient.sex = Sex::Male;
      break;
    case kSexFemale:
      session_.patient.sex = Sex::Female;
      break;
    default:
      session_.patient.sex.reset();
      break;
  }

  // Si la unidad no es kg, mostrar la conversión para que el usuario la vea.
  const bool show_conversion = weight_unit_->currentIndex() != kKilograms && kg > 0.0;
  weight_in_kg_->setText(show_conversion ? "= " + toQString(formatNumber(kg)) + " kg" : QString());

  if (kg <= 0.0) {
    // Aún no se ha digitado el peso: no mostrar errores antes de tiempo.
    messages_->setText(QString());
    complete_ = false;
  } else {
    const std::vector<Warning> warnings = validatePatient(session_.patient);
    messages_->setText(warningsHtml(warnings, palette()));
    complete_ = !hasBlocking(warnings);
  }
  emit completeChanged();
}

}  // namespace calc::gui
