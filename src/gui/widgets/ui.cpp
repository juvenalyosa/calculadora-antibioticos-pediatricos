#include "widgets/ui.hpp"

#include <QFrame>
#include <QLabel>
#include <QStyle>
#include <QVBoxLayout>
#include <QVariant>

namespace calc::gui {

QWidget* makePageHeader(const QString& step, const QString& title, const QString& subtitle,
                        QWidget* parent) {
  auto* header = new QWidget(parent);
  auto* layout = new QVBoxLayout(header);
  layout->setContentsMargins(0, 0, 0, 8);
  layout->setSpacing(2);

  auto* step_label = new QLabel(step.toUpper(), header);
  step_label->setObjectName("fieldLabel");
  auto* title_label = new QLabel(title, header);
  title_label->setObjectName("pageTitle");
  auto* subtitle_label = new QLabel(subtitle, header);
  subtitle_label->setObjectName("pageSubtitle");
  subtitle_label->setWordWrap(true);

  layout->addWidget(step_label);
  layout->addWidget(title_label);
  layout->addWidget(subtitle_label);
  return header;
}

QFrame* makeCard(QWidget* parent, const char* object_name) {
  auto* card = new QFrame(parent);
  card->setObjectName(object_name);
  return card;
}

QLabel* makeFieldLabel(const QString& text, QWidget* parent) {
  auto* label = new QLabel(text, parent);
  label->setObjectName("fieldLabel");
  return label;
}

QLabel* makeHint(const QString& text, QWidget* parent) {
  auto* label = new QLabel(text, parent);
  label->setObjectName("fieldHint");
  label->setWordWrap(true);
  return label;
}

void setStyleProperty(QWidget* widget, const char* name, bool value) {
  if (widget->property(name).toBool() == value && widget->property(name).isValid()) {
    return;
  }
  widget->setProperty(name, value);
  // Las reglas con selector de propiedad solo se reevalúan al repulir.
  widget->style()->unpolish(widget);
  widget->style()->polish(widget);
  widget->update();
}

}  // namespace calc::gui
