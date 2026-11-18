#include "pages/disease_page.hpp"

#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QVBoxLayout>
#include <QWizard>

#include "html.hpp"
#include "theme.hpp"
#include "widgets/ui.hpp"

namespace calc::gui {

namespace {
/// Rol donde cada hoja del árbol guarda el identificador de la enfermedad.
/// Los grupos no lo tienen: así se distinguen de las enfermedades.
constexpr int kDiseaseIdRole = Qt::UserRole;
}  // namespace

DiseasePage::DiseasePage(Session& session, const KnowledgeBase& kb, QWidget* parent)
    : QWizardPage(parent), session_(session), kb_(kb) {
  setButtonText(QWizard::NextButton, "Calcular dosis");

  filter_ = new QLineEdit(this);
  filter_->setObjectName("search");
  filter_->setPlaceholderText("Buscar enfermedad…");
  filter_->setClearButtonEnabled(true);

  tree_ = new QTreeWidget(this);
  tree_->setHeaderHidden(true);
  tree_->setRootIsDecorated(false);
  tree_->setIndentation(0);  // los grupos se distinguen por la tipografía
  tree_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  const Theme& t = theme();

  // Un nodo por grupo, en el orden en que aparecen en la base.
  QMap<QString, QTreeWidgetItem*> groups;
  for (const Disease& disease : kb.diseases) {
    const QString group_name = disease.group.empty() ? "Otras" : toQString(disease.group);
    QTreeWidgetItem* group = groups.value(group_name, nullptr);
    if (group == nullptr) {
      group = new QTreeWidgetItem(tree_, {group_name.toUpper()});
      group->setFlags(Qt::ItemIsEnabled);  // un grupo no se puede seleccionar
      QFont font = group->font(0);
      font.setBold(true);
      font.setPixelSize(11);
      group->setFont(0, font);
      group->setForeground(0, QColor(t.muted));
      groups.insert(group_name, group);
    }

    QString text = toQString(disease.name);
    auto* item = new QTreeWidgetItem(group, {text});
    item->setData(0, kDiseaseIdRole, toQString(disease.id));
    if (!disease.antibiotic_indicated) {
      // Las entidades sin antibiótico se distinguen a la vista.
      item->setText(0, text + "   ·   sin antibiótico");
      item->setForeground(0, QColor(t.muted));
      item->setToolTip(0, toQString(disease.message));
    }
  }
  tree_->expandAll();

  auto* layout = new QVBoxLayout(this);
  layout->setSpacing(10);
  layout->addWidget(makePageHeader(
      "Paso 3 de 4", "Enfermedad",
      "Elija el diagnóstico. Se calcularán todas las líneas de manejo de la guía.", this));
  layout->addWidget(filter_);

  // A la derecha, la ficha de la enfermedad elegida.
  preview_ = new QLabel(this);
  preview_->setWordWrap(true);
  preview_->setTextFormat(Qt::RichText);
  preview_->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  QFrame* preview_card = makeCard(this);
  preview_card->setMinimumWidth(280);
  auto* preview_layout = new QVBoxLayout(preview_card);
  preview_layout->setContentsMargins(18, 16, 18, 16);
  preview_layout->addWidget(preview_);
  preview_layout->addStretch();

  auto* columns = new QHBoxLayout;
  columns->setSpacing(12);
  columns->addWidget(tree_, 3);
  columns->addWidget(preview_card, 2);
  layout->addLayout(columns, 1);
  updatePreview();

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
  updatePreview();
  emit completeChanged();
}

void DiseasePage::updatePreview() {
  if (session_.disease_id.empty()) {
    preview_->setText("<p style='color:" + theme().muted +
                      "'>Elija una enfermedad de la lista para ver sus opciones de tratamiento "
                      "y la guía de la que salen.</p>");
    return;
  }
  preview_->setText(diseasePreviewHtml(kb_, kb_.disease(session_.disease_id)));
}

}  // namespace calc::gui
