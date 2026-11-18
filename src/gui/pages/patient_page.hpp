/// @file patient_page.hpp
/// @brief Paso 1: datos del paciente (edad, peso, talla, sexo).
#pragma once

#include <optional>

#include <QWizardPage>

#include "session.hpp"

class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QSpinBox;

namespace calc::gui {

class AgeBandWidget;

class PatientPage final : public QWizardPage {
  Q_OBJECT

 public:
  explicit PatientPage(Session& session, QWidget* parent = nullptr);

  /// «Siguiente» solo se habilita con datos sin advertencias bloqueantes.
  [[nodiscard]] bool isComplete() const override;

  /// Carga valores sin intervención del usuario (pruebas y capturas).
  /// Una talla de 0 significa «no registrada»; un sexo vacío, «no registrado».
  void setValues(int years, int months, double weight_kg, double height_cm,
                 std::optional<Sex> sex = std::nullopt);

 private:
  /// Peso digitado, convertido a kilogramos según la unidad elegida.
  [[nodiscard]] double weightInKilograms() const;
  /// Ajusta rango y decimales del campo de peso a la unidad elegida.
  void applyWeightUnit();
  /// Widgets → sesión, validación y mensajes. Se llama en cada cambio.
  void refresh();

  Session& session_;
  QSpinBox* years_ = nullptr;
  QSpinBox* months_ = nullptr;
  QDoubleSpinBox* weight_ = nullptr;
  QComboBox* weight_unit_ = nullptr;
  QLabel* weight_in_kg_ = nullptr;
  QDoubleSpinBox* height_ = nullptr;
  QComboBox* sex_ = nullptr;
  AgeBandWidget* age_band_ = nullptr;
  QLabel* expected_weight_ = nullptr;
  QLabel* messages_ = nullptr;
  bool complete_ = false;
};

}  // namespace calc::gui
