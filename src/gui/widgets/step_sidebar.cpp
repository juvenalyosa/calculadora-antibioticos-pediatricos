#include "widgets/step_sidebar.hpp"

#include <utility>

#include <QFont>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>

#include "theme.hpp"

namespace calc::gui {

namespace {
constexpr int kWidth = 250;
constexpr int kMargin = 24;
constexpr int kStepsTop = 168;
constexpr int kStepHeight = 52;
constexpr int kCircle = 28;
}  // namespace

StepSidebar::StepSidebar(QStringList steps, QString app_version, QString kb_version,
                         QWidget* parent)
    : QWidget(parent),
      steps_(std::move(steps)),
      app_version_(std::move(app_version)),
      kb_version_(std::move(kb_version)) {
  setFixedWidth(kWidth);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void StepSidebar::setCurrentStep(int index) {
  if (index != current_) {
    current_ = index;
    update();
  }
}

QSize StepSidebar::sizeHint() const {
  return {kWidth, 600};
}

void StepSidebar::paintEvent(QPaintEvent* /*event*/) {
  const Theme& t = theme();
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  // Panel con esquinas redondeadas: QWizard deja un margen a su alrededor.
  QPainterPath panel;
  panel.addRoundedRect(QRectF(rect()), 12, 12);
  QLinearGradient shade(0, 0, 0, height());
  shade.setColorAt(0.0, QColor(t.sidebar).lighter(118));
  shade.setColorAt(1.0, QColor(t.sidebar));
  painter.fillPath(panel, shade);

  QFont base = font();

  // --- Identidad ---
  makeAppIcon().paint(&painter, QRect(kMargin, 28, 44, 44));

  QFont title = base;
  title.setPixelSize(17);
  title.setBold(true);
  painter.setFont(title);
  painter.setPen(QColor(t.sidebar_text));
  painter.drawText(QRect(kMargin, 84, kWidth - 2 * kMargin, 46), Qt::AlignLeft | Qt::TextWordWrap,
                   "Calculadora de dosis de antibióticos");

  QFont small = base;
  small.setPixelSize(12);
  painter.setFont(small);
  painter.setPen(QColor(t.sidebar_muted));
  painter.drawText(QRect(kMargin, 132, kWidth - 2 * kMargin, 18), Qt::AlignLeft, "Pediatría");

  // --- Pasos ---
  for (int i = 0; i < steps_.size(); ++i) {
    const int top = kStepsTop + i * kStepHeight;
    const QRect row(12, top, kWidth - 24, kStepHeight - 8);
    const bool current = i == current_;
    const bool done = i < current_;

    if (current) {
      QPainterPath highlight;
      highlight.addRoundedRect(row, 8, 8);
      painter.fillPath(highlight, QColor(t.sidebar_active));
    }

    // Línea que une este paso con el siguiente.
    const QPoint center(kMargin + kCircle / 2 + 2, row.center().y() + 1);
    if (i + 1 < steps_.size()) {
      painter.setPen(QPen(QColor(done ? t.sidebar_text : t.sidebar_muted), 1.5));
      painter.drawLine(center.x(), center.y() + kCircle / 2 + 2, center.x(),
                       center.y() + kStepHeight - kCircle / 2 - 2);
    }

    // Círculo: lleno si el paso está hecho o es el actual; solo borde si falta.
    const QRectF circle(center.x() - kCircle / 2.0, center.y() - kCircle / 2.0, kCircle, kCircle);
    if (done || current) {
      painter.setPen(Qt::NoPen);
      painter.setBrush(QColor(t.sidebar_text));
    } else {
      painter.setPen(QPen(QColor(t.sidebar_muted), 1.5));
      painter.setBrush(Qt::NoBrush);
    }
    painter.drawEllipse(circle);

    QFont number = base;
    number.setPixelSize(13);
    number.setBold(true);
    painter.setFont(number);
    painter.setPen(QColor(done || current ? t.sidebar : t.sidebar_muted));
    painter.drawText(circle, Qt::AlignCenter, done ? QStringLiteral("✓") : QString::number(i + 1));

    QFont label = base;
    label.setPixelSize(14);
    label.setBold(current);
    painter.setFont(label);
    painter.setPen(QColor(current || done ? t.sidebar_text : t.sidebar_muted));
    painter.drawText(QRect(center.x() + kCircle / 2 + 12, row.top(), row.width() - 60, row.height()),
                     Qt::AlignVCenter | Qt::AlignLeft, steps_.at(i));
  }

  // --- Pie: versiones ---
  painter.setFont(small);
  painter.setPen(QColor(t.sidebar_muted));
  const QString footer = "Versión " + app_version_ + "\nBase de conocimiento " + kb_version_ +
                         "\n\nHerramienta de apoyo académico. No reemplaza el juicio clínico.";
  painter.drawText(QRect(kMargin, height() - 118, kWidth - 2 * kMargin, 100),
                   Qt::AlignLeft | Qt::AlignBottom | Qt::TextWordWrap, footer);
}

}  // namespace calc::gui
