#include "widgets/ui.hpp"

#include "widgets/hero_header.hpp"

#include <QFrame>
#include <QLabel>
#include <QStyle>
#include <QVBoxLayout>
#include <QVariant>

namespace calc::gui {

QWidget* makePageHeader(int step, const QString& title, const QString& subtitle,
                        QWidget* parent) {
  return new HeroHeader(step, title, subtitle, parent);
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
