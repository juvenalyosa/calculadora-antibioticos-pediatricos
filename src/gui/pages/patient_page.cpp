#include "pages/patient_page.hpp"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

#include "calc/text.hpp"
#include "html.hpp"
#include "widgets/age_band.hpp"
#include "widgets/ui.hpp"

namespace calc::gui {

namespace {
// Unidades de peso que ofrece el selector (índices del QComboBox).
enum WeightUnit { kKilograms = 0, kPounds = 1, kGrams = 2 };
// Opciones del selector de sexo (índices del QComboBox).
enum SexChoice { kSexNotRecorded = 0, kSexMale = 1, kSexFemale = 2 };
}  // namespace

PatientPage::PatientPage(Session& session, QWidget* parent)
    : QWizardPage(parent), session_(session) {
  // --- Edad ---
  years_ = new QSpinBox(this);
  years_->setRange(0, 17);
  years_->setSuffix(" años");
  years_->setMinimumWidth(120);
  months_ = new QSpinBox(this);
  months_->setRange(0, 11);
  months_->setSuffix(" meses");
  months_->setMinimumWidth(120);
  auto* age_row = new QHBoxLayout;
  age_row->setSpacing(8);
  age_row->addWidget(years_);
  age_row->addWidget(months_);
  age_row->addStretch();

  // --- Peso, con selector de unidad ---
  weight_ = new QDoubleSpinBox(this);
  weight_->setMinimumWidth(120);
  weight_unit_ = new QComboBox(this);
  weight_unit_->addItems({"kg", "lb", "g"});
  weight_unit_->setMinimumWidth(70);
  weight_in_kg_ = makeHint(QString(), this);
  weight_in_kg_->setWordWrap(false);
  auto* weight_row = new QHBoxLayout;
  weight_row->setSpacing(8);
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
  height_->setMinimumWidth(160);
  auto* height_row = new QHBoxLayout;
  height_row->addWidget(height_);
  height_row->addStretch();

  // --- Sexo (opcional; lo usa la ecuación CKiD U25) ---
  sex_ = new QComboBox(this);
  sex_->addItems({"No registrado", "Masculino", "Femenino"});
  sex_->setMinimumWidth(160);
  auto* sex_row = new QHBoxLayout;
  sex_row->addWidget(sex_);
  sex_row->addStretch();

  // Tarjeta con dos bloques: lo obligatorio y lo que solo sirve para la TFG.
  auto* card = makeCard(this);
  auto* grid = new QGridLayout(card);
  grid->setContentsMargins(20, 18, 20, 18);
  grid->setHorizontalSpacing(16);
  grid->setVerticalSpacing(12);
  grid->setColumnMinimumWidth(0, 70);
  grid->setColumnStretch(1, 1);

  auto* required_title = new QLabel("Datos para calcular la dosis", card);
  required_title->setObjectName("cardTitle");
  auto* optional_title = new QLabel("Datos para estimar la función renal", card);
  optional_title->setObjectName("cardTitle");
  auto* separator = new QFrame(card);
  separator->setObjectName("separator");

  int row = 0;
  grid->addWidget(required_title, row++, 0, 1, 2);
  grid->addWidget(makeFieldLabel("EDAD", card), row, 0);
  grid->addLayout(age_row, row++, 1);
  grid->addWidget(makeFieldLabel("PESO", card), row, 0);
  grid->addLayout(weight_row, row++, 1);
  grid->addWidget(separator, row++, 0, 1, 2);
  grid->addWidget(optional_title, row++, 0, 1, 2);
  grid->addWidget(makeHint("Opcionales. Solo se usan si en el paso siguiente estima la TFG con "
                           "la creatinina.", card), row++, 0, 1, 2);
  grid->addWidget(makeFieldLabel("TALLA", card), row, 0);
  grid->addLayout(height_row, row++, 1);
  grid->addWidget(makeFieldLabel("SEXO", card), row, 0);
  grid->addLayout(sex_row, row++, 1);

  messages_ = new QLabel(this);
  messages_->setObjectName("messages");
  messages_->setWordWrap(true);
  messages_->setTextFormat(Qt::RichText);

  // Aviso permanente en la primera pantalla.
  auto* disclaimer = new QLabel(
      "Herramienta de apoyo académico. No reemplaza el juicio clínico ni la verificación de la "
      "dosis antes de formular. No se guarda ningún dato que identifique al paciente.",
      this);
  disclaimer->setObjectName("notice");
  disclaimer->setWordWrap(true);

  auto* layout = new QVBoxLayout(this);
  layout->setSpacing(10);
  layout->addWidget(makePageHeader(0, "Paciente",
                                   "Ingrese la edad y el peso del paciente.", this));
  // A la derecha, el grupo de edad y el peso esperado: ayudan a notar a
  // simple vista una edad o un peso mal digitados.
  auto* age_card = makeCard(this);
  age_card->setMinimumWidth(320);
  auto* age_layout = new QVBoxLayout(age_card);
  age_layout->setContentsMargins(20, 18, 20, 18);
  age_layout->setSpacing(6);
  auto* age_title = new QLabel("Grupo de edad", age_card);
  age_title->setObjectName("cardTitle");
  age_band_ = new AgeBandWidget(age_card);
  expected_weight_ = makeHint(QString(), age_card);
  age_layout->addWidget(age_title);
  age_layout->addWidget(age_band_);
  age_layout->addWidget(expected_weight_);
  age_layout->addStretch();

  auto* columns = new QHBoxLayout;
  columns->setSpacing(12);
  columns->addWidget(card, 3);
  columns->addWidget(age_card, 2);

  layout->addLayout(columns);
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

  // «1 año», «2 años»; «1 mes», «2 meses».
  years_->setSuffix(years_->value() == 1 ? " año" : " años");
  months_->setSuffix(months_->value() == 1 ? " mes" : " meses");

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

  const int total_months = session_.patient.age.totalMonths();
  age_band_->setAgeMonths(total_months);
  if (total_months >= limits::kMinAgeMonths) {
    expected_weight_->setText(
        "<b>" + AgeBandWidget::bandName(total_months) + ".</b> Peso esperado para la edad: cerca de " +
        toQString(formatNumber(expectedWeightForAge(session_.patient.age).value(), 1)) +
        " kg (valor orientativo).");
  } else {
    expected_weight_->setText("La calculadora no cubre a los menores de 1 mes.");
  }

  // Si la unidad no es kg, mostrar la conversión para que el usuario la vea.
  const bool show_conversion = weight_unit_->currentIndex() != kKilograms && kg > 0.0;
  weight_in_kg_->setText(show_conversion ? "= " + toQString(formatNumber(kg)) + " kg" : QString());

  if (kg <= 0.0) {
    // Aún no se ha digitado el peso: no mostrar errores antes de tiempo.
    messages_->setText(QString());
    messages_->setVisible(!messages_->text().isEmpty());
    complete_ = false;
  } else {
    const std::vector<Warning> warnings = validatePatient(session_.patient);
    messages_->setText(warningsHtml(warnings));
    messages_->setVisible(!messages_->text().isEmpty());
    complete_ = !hasBlocking(warnings);
  }
  emit completeChanged();
}

}  // namespace calc::gui
