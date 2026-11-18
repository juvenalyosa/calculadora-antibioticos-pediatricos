#include "widgets/age_band.hpp"

#include <array>

#include <QFont>
#include <QPainter>
#include <QPainterPath>

#include "theme.hpp"

namespace calc::gui {

namespace {

struct Band {
  const char* name;
  const char* range;
  int from_months;   // incluido
  double height;     // altura de la figura, en píxeles
};

// Grupos de edad de uso habitual en pediatría. Solo rotulan el dibujo.
constexpr std::array<Band, 4> kBands = {{
    {"Lactante", "1 a 23 meses", 0, 46.0},
    {"Preescolar", "2 a 5 años", 24, 64.0},
    {"Escolar", "6 a 11 años", 72, 84.0},
    {"Adolescente", "12 a 17 años", 144, 104.0},
}};

int bandIndex(int months) {
  int index = 0;
  for (int i = 0; i < static_cast<int>(kBands.size()); ++i) {
    if (months >= kBands[static_cast<std::size_t>(i)].from_months) {
      index = i;
    }
  }
  return index;
}

/// Figura sencilla: cabeza redonda y cuerpo en forma de campana.
void drawFigure(QPainter& painter, QPointF feet, double height, const QColor& color) {
  const double head = height * 0.19;                // radio de la cabeza
  const double body_top = feet.y() - height + head * 2.0 + 3.0;
  const double half = height * 0.24;                // medio ancho del cuerpo en la base

  painter.setPen(Qt::NoPen);
  painter.setBrush(color);
  painter.drawEllipse(QPointF(feet.x(), feet.y() - height + head), head, head);

  QPainterPath body;
  body.moveTo(feet.x() - half, feet.y());
  body.cubicTo(feet.x() - half, body_top + 6, feet.x() - half * 0.5, body_top, feet.x(), body_top);
  body.cubicTo(feet.x() + half * 0.5, body_top, feet.x() + half, body_top + 6, feet.x() + half,
               feet.y());
  body.closeSubpath();
  painter.drawPath(body);
}

}  // namespace

AgeBandWidget::AgeBandWidget(QWidget* parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void AgeBandWidget::setAgeMonths(int months) {
  if (months != months_) {
    months_ = months;
    update();
  }
}

QString AgeBandWidget::bandName(int months) {
  if (months < 1) {
    return QStringLiteral("Recién nacido");
  }
  return QString::fromUtf8(kBands[static_cast<std::size_t>(bandIndex(months))].name);
}

QSize AgeBandWidget::sizeHint() const {
  return {300, 168};
}

void AgeBandWidget::paintEvent(QPaintEvent* /*event*/) {
  const Theme& t = theme();
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  const int count = static_cast<int>(kBands.size());
  const double slot = width() / static_cast<double>(count);
  const double floor_y = 118.0;
  const int current = months_ >= 1 ? bandIndex(months_) : -1;

  // Suelo: una línea suave bajo las figuras.
  painter.setPen(QPen(QColor(t.border), 2, Qt::SolidLine, Qt::RoundCap));
  painter.drawLine(QPointF(12, floor_y + 1), QPointF(width() - 12, floor_y + 1));

  QFont name = font();
  name.setPixelSize(12);
  QFont range = font();
  range.setPixelSize(10);

  for (int i = 0; i < count; ++i) {
    const Band& band = kBands[static_cast<std::size_t>(i)];
    const double x = slot * (i + 0.5);
    const bool active = i == current;

    if (active) {
      // Resalte detrás de la figura del grupo actual.
      QPainterPath halo;
      halo.addRoundedRect(QRectF(x - slot / 2 + 4, 4, slot - 8, floor_y + 44), 14, 14);
      painter.fillPath(halo, QColor(t.primary_soft));
    }
    drawFigure(painter, {x, floor_y}, band.height, QColor(active ? t.primary : t.border));

    name.setBold(active);
    painter.setFont(name);
    painter.setPen(QColor(active ? t.text : t.muted));
    painter.drawText(QRectF(x - slot / 2, floor_y + 8, slot, 16), Qt::AlignHCenter | Qt::AlignTop,
                     QString::fromUtf8(band.name));
    painter.setFont(range);
    painter.setPen(QColor(t.muted));
    painter.drawText(QRectF(x - slot / 2, floor_y + 25, slot, 14), Qt::AlignHCenter | Qt::AlignTop,
                     QString::fromUtf8(band.range));
  }
}

}  // namespace calc::gui
