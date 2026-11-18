/// @file disease_page.hpp
/// @brief Paso 3: elección de la enfermedad.
#pragma once

#include <string>

#include <QWizardPage>

#include "calc/knowledge_base.hpp"
#include "session.hpp"

class QLabel;
class QLineEdit;
class QTreeWidget;

namespace calc::gui {

class DiseasePage final : public QWizardPage {
  Q_OBJECT

 public:
  DiseasePage(Session& session, const KnowledgeBase& kb, QWidget* parent = nullptr);

  [[nodiscard]] bool isComplete() const override;

  /// Selecciona una enfermedad por identificador (pruebas y capturas).
  void selectDisease(const std::string& disease_id);

 private:
  /// Oculta las enfermedades que no contienen el texto buscado.
  void applyFilter(const QString& text);
  /// Selección del árbol → sesión.
  void onSelectionChanged();
  /// Muestra la ficha de la enfermedad elegida.
  void updatePreview();

  Session& session_;
  const KnowledgeBase& kb_;
  QLabel* preview_ = nullptr;
  QLineEdit* filter_ = nullptr;
  QTreeWidget* tree_ = nullptr;
};

}  // namespace calc::gui
