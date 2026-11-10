#include "report_dialog.hpp"

#include <exception>
#include <utility>

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "html.hpp"

namespace calc::gui {

ReportDialog::ReportDialog(ErrorReport report, ReportSink& sink, QWidget* parent)
    : QDialog(parent), report_(std::move(report)), sink_(sink) {
  setWindowTitle("Reportar un error");
  setMinimumWidth(480);

  auto* intro = new QLabel(
      "Describa qué encontró. Los datos del cálculo se adjuntan automáticamente; "
      "no se guarda ningún dato que identifique al paciente.",
      this);
  intro->setWordWrap(true);

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
  comment_->setMinimumHeight(90);

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

  auto* form = new QFormLayout;
  form->addRow("Tipo de problema:", category_);
  form->addRow("Comentario:", comment_);
  form->addRow("Se adjunta:", attached_view);

  auto* buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
  buttons->button(QDialogButtonBox::Save)->setText("Guardar reporte");
  buttons->button(QDialogButtonBox::Cancel)->setText("Cancelar");
  connect(buttons, &QDialogButtonBox::accepted, this, &ReportDialog::save);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  auto* layout = new QVBoxLayout(this);
  layout->addWidget(intro);
  layout->addLayout(form);
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
