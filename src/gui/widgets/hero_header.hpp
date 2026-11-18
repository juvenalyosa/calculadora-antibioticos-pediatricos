/// @file hero_header.hpp
/// @brief Encabezado de página: banda de color con el título, burbujas
///        decorativas y el progreso por pasos.
#pragma once

#include <QString>
#include <QStringList>
#include <QWidget>

namespace calc::gui {

/// Banda redondeada que encabeza cada paso. A la izquierda van la marca, el
/// título y una línea de ayuda; a la derecha, los pasos (hechos, actual y
/// pendientes). Con `step` negativo no dibuja los pasos (diálogos).
class HeroHeader final : public QWidget {
  Q_OBJECT

 public:
  HeroHeader(int step, QString title, QString subtitle, QWidget* parent = nullptr);

  [[nodiscard]] QSize sizeHint() const override;
  [[nodiscard]] QSize minimumSizeHint() const override;

  /// Nombres de los pasos del asistente, en orden.
  [[nodiscard]] static const QStringList& stepNames();

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  int step_;
  QString title_;
  QString subtitle_;
};

}  // namespace calc::gui
