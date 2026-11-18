/// @file age_band.hpp
/// @brief Dibujo del grupo de edad del paciente (lactante, preescolar,
///        escolar o adolescente).
#pragma once

#include <QString>
#include <QWidget>

namespace calc::gui {

/// Cuatro figuras de tamaño creciente; se resalta la del grupo de edad del
/// paciente. Es una ayuda visual: no interviene en ningún cálculo.
class AgeBandWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit AgeBandWidget(QWidget* parent = nullptr);

  /// Edad en meses cumplidos.
  void setAgeMonths(int months);

  /// Nombre del grupo de edad: «Lactante», «Preescolar», «Escolar» o
  /// «Adolescente»; «Recién nacido» por debajo de un mes.
  [[nodiscard]] static QString bandName(int months);

  [[nodiscard]] QSize sizeHint() const override;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  int months_ = 0;
};

}  // namespace calc::gui
