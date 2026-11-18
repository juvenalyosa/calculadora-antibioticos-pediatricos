/// @file step_sidebar.hpp
/// @brief Barra lateral: identidad de la aplicación y progreso por pasos.
#pragma once

#include <QStringList>
#include <QWidget>

namespace calc::gui {

/// Muestra el nombre de la aplicación, la lista de pasos (hechos, actual y
/// pendientes) y, al pie, las versiones de la aplicación y de la base.
class StepSidebar final : public QWidget {
  Q_OBJECT

 public:
  StepSidebar(QStringList steps, QString app_version, QString kb_version,
              QWidget* parent = nullptr);

  /// Índice del paso actual (0 = primero).
  void setCurrentStep(int index);

  [[nodiscard]] QSize sizeHint() const override;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QStringList steps_;
  QString app_version_;
  QString kb_version_;
  int current_ = 0;
};

}  // namespace calc::gui
