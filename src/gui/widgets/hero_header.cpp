#include "widgets/hero_header.hpp"

#include <utility>

#include <QFont>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>

#include "theme.hpp"

namespace calc::gui {

namespace {
constexpr int kHeight = 132;
constexpr int kNode = 34;      // diámetro del círculo de cada paso
constexpr int kNodeGap = 92;   // distancia entre centros de pasos

/// Burbuja decorativa: círculo translúcido con un brillo arriba a la izquierda.
void drawBubble(QPainter& painter, QPointF center, double radius, int alpha) {
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(255, 255, 255, alpha));
  painter.drawEllipse(center, radius, radius);
  painter.setBrush(QColor(255, 255, 255, alpha + 10));
  painter.drawEllipse(center + QPointF(-radius * 0.35, -radius * 0.35), radius * 0.2, radius * 0.2);
}
}  // namespace

HeroHeader::HeroHeader(int step, QString title, QString subtitle, QWidget* parent)
    : QWidget(parent), step_(step), title_(std::move(title)), subtitle_(std::move(subtitle)) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

QSize HeroHeader::sizeHint() const {
  return {720, kHeight};
}

QSize HeroHeader::minimumSizeHint() const {
  return {360, kHeight};
}

const QStringList& HeroHeader::stepNames() {
  static const QStringList names = {"Paciente", "Función renal", "Enfermedad", "Resultado"};
  return names;
}

void HeroHeader::paintEvent(QPaintEvent* /*event*/) {
  const Theme& t = theme();
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Fondo: banda redondeada, del petróleo al teal.
  QPainterPath band;
  band.addRoundedRect(QRectF(rect()), 24, 24);
  QLinearGradient gradient(0, 0, width(), 0);
  gradient.setColorAt(0.0, QColor(t.sidebar));
  gradient.setColorAt(1.0, QColor(t.sidebar_active));
  painter.fillPath(band, gradient);

  // Burbujas: posiciones fijas respecto al borde derecho, para que el dibujo
  // sea el mismo en todas las páginas.
  painter.setClipPath(band);
  const double w = width();
  drawBubble(painter, {w - 60, 18}, 46, 9);
  drawBubble(painter, {w - 190, kHeight + 8}, 38, 8);
  drawBubble(painter, {w - 330, 6}, 20, 10);
  drawBubble(painter, {w * 0.46, kHeight - 6}, 28, 7);
  drawBubble(painter, {w * 0.36, 20}, 11, 10);
  drawBubble(painter, {26, kHeight + 4}, 34, 7);
  painter.setClipping(false);

  const bool with_steps = step_ >= 0;
  const int steps = static_cast<int>(stepNames().size());
  const int steps_width = with_steps ? (steps - 1) * kNodeGap + kNode + 56 : 0;
  const int text_left = 28;
  const int text_width = qMax(180, width() - text_left - steps_width - 16);

  // --- Marca, título y ayuda ---
  QFont brand = font();
  brand.setPixelSize(12);
  brand.setBold(true);
  painter.setFont(brand);
  painter.setPen(QColor(t.sidebar_muted));
  makeAppIcon().paint(&painter, QRect(text_left, 18, 20, 20));
  painter.drawText(QRect(text_left + 28, 18, text_width - 28, 20), Qt::AlignLeft | Qt::AlignVCenter,
                   "Calculadora de antibióticos · Pediatría");

  // Línea fina de acento bajo la marca.
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(t.accent));
  painter.drawRoundedRect(QRectF(text_left, 42, 34, 2), 1, 1);

  QFont title = font();
  title.setPixelSize(27);
  title.setBold(true);
  painter.setFont(title);
  painter.setPen(QColor(t.sidebar_text));
  painter.drawText(QRect(text_left, 48, text_width, 36), Qt::AlignLeft | Qt::AlignVCenter, title_);

  QFont subtitle = font();
  subtitle.setPixelSize(14);
  painter.setFont(subtitle);
  painter.setPen(QColor(t.sidebar_muted));
  painter.drawText(QRect(text_left, 86, text_width, 40),
                   Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, subtitle_);

  if (!with_steps) {
    return;
  }

  // --- Pasos ---
  const int first_x = width() - steps_width + 28 + kNode / 2;
  const int node_y = 50;
  QFont number = font();
  number.setPixelSize(14);
  number.setBold(true);
  QFont label = font();
  label.setPixelSize(11);

  for (int i = 0; i < steps; ++i) {
    const QPointF center(first_x + i * kNodeGap, node_y);
    const bool done = i < step_;
    const bool current = i == step_;

    // Tramo punteado hacia el paso siguiente; continuo si ya se recorrió.
    if (i + 1 < steps) {
      QPen line(QColor(255, 255, 255, done ? 230 : 110), 2, done ? Qt::SolidLine : Qt::DotLine,
                Qt::RoundCap);
      painter.setPen(line);
      painter.drawLine(center + QPointF(kNode / 2.0 + 6, 0),
                       center + QPointF(kNodeGap - kNode / 2.0 - 6, 0));
    }

    if (current) {
      // Aro fino del color de acento alrededor del paso actual.
      painter.setPen(QPen(QColor(t.accent), 2));
      painter.setBrush(Qt::NoBrush);
      painter.drawEllipse(center, kNode / 2.0 + 5, kNode / 2.0 + 5);
    }
    if (done || current) {
      painter.setPen(Qt::NoPen);
      painter.setBrush(QColor(current ? "#ffffff" : t.window));
    } else {
      painter.setPen(QPen(QColor(255, 255, 255, 150), 2));
      painter.setBrush(Qt::NoBrush);
    }
    painter.drawEllipse(center, kNode / 2.0, kNode / 2.0);

    painter.setFont(number);
    painter.setPen(done || current ? QColor(t.sidebar) : QColor(255, 255, 255, 190));
    painter.drawText(QRectF(center.x() - kNode / 2.0, center.y() - kNode / 2.0, kNode, kNode),
                     Qt::AlignCenter, done ? QStringLiteral("✓") : QString::number(i + 1));

    label.setBold(current);
    painter.setFont(label);
    painter.setPen(current || done ? QColor(t.sidebar_text) : QColor(255, 255, 255, 170));
    painter.drawText(QRectF(center.x() - kNodeGap / 2.0, node_y + kNode / 2.0 + 10, kNodeGap, 18),
                     Qt::AlignHCenter | Qt::AlignTop, stepNames().at(i));
  }
}

}  // namespace calc::gui
