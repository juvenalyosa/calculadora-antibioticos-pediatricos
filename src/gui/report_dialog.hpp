/// @file report_dialog.hpp
/// @brief Formato semiautomatizado para reportar un error desde cualquier paso.
#pragma once

#include <QDialog>

#include "calc/error_report.hpp"

class QComboBox;
class QPlainTextEdit;

namespace calc::gui {

/// Muestra los parámetros capturados automáticamente y pide al usuario solo
/// el tipo de problema y un comentario. Al guardar, entrega el reporte al
/// destino configurado.
class ReportDialog final : public QDialog {
  Q_OBJECT

 public:
  /// @param report  Reporte ya armado con makeErrorReport (sin comentario).
  /// @param sink    Destino del reporte; debe vivir más que el diálogo.
  ReportDialog(ErrorReport report, ReportSink& sink, QWidget* parent = nullptr);

 private:
  /// Completa el reporte con lo escrito y lo entrega.
  void save();

  ErrorReport report_;
  ReportSink& sink_;
  QComboBox* category_ = nullptr;
  QPlainTextEdit* comment_ = nullptr;
};

}  // namespace calc::gui
