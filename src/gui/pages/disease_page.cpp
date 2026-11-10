#include "pages/disease_page.hpp"

#include <QHeaderView>
#include <QLineEdit>
#include <QMap>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QVBoxLayout>
#include <QWizard>

#include "html.hpp"

namespace calc::gui {

namespace {
/// Rol donde cada hoja del árbol guarda el identificador de la enfermedad.
/// Los grupos no lo tienen: así se distinguen de las enfermedades.
constexpr int kDiseaseIdRole = Qt::UserRole;
}  // namespace

DiseasePage::DiseasePage(Session& session, const KnowledgeBase& kb, QWidget* parent)
    : QWizardPage(parent), session_(session) {
  setTitle("Paso 3 de 4 · Enfermedad");
  setSubTitle("Elija el diagnóstico. Se calcularán todas las líneas de manejo de la guía.");
  setButtonText(QWizard::NextButton, "Calcular >");

  filter_ = new QLineEdit(this);
  filter_->setPlaceholderText("Buscar enfermedad…");
  filter_->setClearButtonEnabled(true);

  tree_ = new QTreeWidget(this);
  tree_->setHeaderHidden(true);
  tree_->setRootIsDecorated(false);

  // Un nodo por grupo, en el orden en que aparecen en la base.
  QMap<QString, QTreeWidgetItem*> groups;
  for (const Disease& disease : kb.diseases) {
    const QString group_name = disease.group.empty() ? "Otras" : toQString(disease.group);
    QTreeWidgetItem* group = groups.value(group_name, nullptr);
    if (group == nullptr) {
      group = new QTreeWidgetItem(tree_, {group_name});
      group->setFlags(Qt::ItemIsEnabled);  // un grupo no se puede seleccionar
      QFont font = group->font(0);
      font.setBold(true);
      group->setFont(0, font);
      groups.insert(group_name, group);
    }

    QString text = toQString(disease.name);
    if (!disease.antibiotic_indicated) {
      text += " — sin antibiótico";
    }
    auto* item = new QTreeWidgetItem(group, {text});
    item->setData(0, kDiseaseIdRole, toQString(disease.id));
  }
  tree_->expandAll();

  auto* layout = new QVBoxLayout(this);
  layout->addWidget(filter_);
  layout->addWidget(tree_);

  connect(filter_, &QLineEdit::textChanged, this, &DiseasePage::applyFilter);
  connect(tree_, &QTreeWidget::itemSelectionChanged, this, &DiseasePage::onSelectionChanged);
  // Doble clic sobre una enfermedad equivale a elegirla y pulsar «Siguiente».
  connect(tree_, &QTreeWidget::itemDoubleClicked, this, [this](QTreeWidgetItem* item) {
    if (item->data(0, kDiseaseIdRole).isValid() && wizard() != nullptr) {
      wizard()->next();
    }
  });
}

bool DiseasePage::isComplete() const {
  return !session_.disease_id.empty();
}

void DiseasePage::selectDisease(const std::string& disease_id) {
  for (QTreeWidgetItemIterator it(tree_); *it != nullptr; ++it) {
    if ((*it)->data(0, kDiseaseIdRole).toString() == toQString(disease_id)) {
      tree_->setCurrentItem(*it);
      return;
    }
  }
}

void DiseasePage::applyFilter(const QString& text) {
  for (int g = 0; g < tree_->topLevelItemCount(); ++g) {
    QTreeWidgetItem* group = tree_->topLevelItem(g);
    bool any_visible = false;
    for (int i = 0; i < group->childCount(); ++i) {
      QTreeWidgetItem* item = group->child(i);
      const bool matches = item->text(0).contains(text, Qt::CaseInsensitive);
      item->setHidden(!matches);
      any_visible = any_visible || matches;
    }
    group->setHidden(!any_visible);
  }
}

void DiseasePage::onSelectionChanged() {
  const QList<QTreeWidgetItem*> selected = tree_->selectedItems();
  session_.disease_id =
      selected.isEmpty() ? std::string() : selected.first()->data(0, kDiseaseIdRole).toString().toStdString();
  emit completeChanged();
}

}  // namespace calc::gui
