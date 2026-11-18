#include "report_dialog.hpp"

#include <exception>
#include <utility>

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "html.hpp"
#include "widgets/ui.hpp"

namespace calc::gui {

ReportDialog::ReportDialog(ErrorReport report, ReportSink& sink, QWidget* parent)
    : QDialog(parent), report_(std::move(report)), sink_(sink) {
  setWindowTitle("Reportar un error");
  setMinimumWidth(560);

  category_ = new QComboBox(this);
  category_->addItem("La dosis calculada parece incorrecta",
                     static_cast<int>(ReportCategory::CalculationError));
  category_->addItem("El contenido de la guía parece incorrecto o desactualizado",
                     static_cast<int>(ReportCategory::ClinicalContent));
  category_->addItem("Problema con la interfaz o su claridad",
                     static_cast<int>(ReportCategory::Usability));
  category_->addItem("Otro", static_cast<int>(ReportCategory::Other));

  comment_ = new QPlainTextEdit(this);
  comment_->setPlaceholderText("Por ejemplo: «La guía indica 80 mg/kg/día y aquí aparece 90».");
  comment_->setMinimumHeight(96);

  // Lo que se adjunta, a la vista del usuario antes de guardar.
  QString attached = "Paso: " + toQString(report_.step) + "\nVersión de la aplicación: " +
                     toQString(report_.app_version) + "\nVersión de la base: " +
                     toQString(report_.kb_version);
  for (const auto& [key, value] : report_.context) {
    attached += "\n" + toQString(key) + ": " + toQString(value);
  }
  auto* attached_view = new QPlainTextEdit(attached, this);
  attached_view->setReadOnly(true);
  attached_view->setMaximumHeight(170);

  auto* card = makeCard(this);
  auto* form = new QVBoxLayout(card);
  form->setContentsMargins(20, 16, 20, 18);
  form->setSpacing(6);
  form->addWidget(makeFieldLabel("TIPO DE PROBLEMA", card));
  form->addWidget(category_);
  form->addSpacing(8);
  form->addWidget(makeFieldLabel("COMENTARIO", card));
  form->addWidget(comment_);
  form->addSpacing(8);
  form->addWidget(makeFieldLabel("SE ADJUNTA AUTOMÁTICAMENTE", card));
  form->addWidget(attached_view);

  auto* buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
  buttons->button(QDialogButtonBox::Save)->setText("Guardar reporte");
  buttons->button(QDialogButtonBox::Save)->setDefault(true);
  buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");
  connect(buttons, &QDialogButtonBox::accepted, this, &ReportDialog::save);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  auto* layout = new QVBoxLayout(this);
  layout->setContentsMargins(24, 22, 24, 20);
  layout->setSpacing(12);
  layout->addWidget(makePageHeader(
      -1, "Reportar un error",
      "Describa qué encontró. Los datos del cálculo se adjuntan automáticamente; no se guarda "
      "ningún dato que identifique al paciente.",
      this));
  layout->addWidget(card);
  layout->addWidget(buttons);
}

void ReportDialog::save() {
  report_.category = static_cast<ReportCategory>(category_->currentData().toInt());
  report_.comment = comment_->toPlainText().trimmed().toStdString();

  try {
    sink_.submit(report_);
  } catch (const std::exception& error) {
    // El reporte no se pierde en silencio: el diálogo sigue abierto.
    QMessageBox::warning(this, "No se pudo guardar el reporte", toQString(error.what()));
    return;
  }
  QMessageBox::information(this, "Reporte guardado",
                           "Gracias. El reporte quedó registrado para su revisión.");
  accept();
}

}  // namespace calc::gui
