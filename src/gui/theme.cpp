#include "theme.hpp"

#include <QApplication>
#include <QColor>
#include <QFont>
#include <QFontDatabase>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPixmap>
#include <QStyleFactory>
#include <QStyleHints>
#include <QTemporaryDir>

namespace calc::gui {

namespace {

Theme lightTheme() {
  Theme t;
  t.dark = false;
  t.window = "#f3f6f9";
  t.surface = "#ffffff";
  t.surface_alt = "#eef2f6";
  t.border = "#d5dde5";
  t.text = "#18212b";
  t.muted = "#5a6876";
  t.disabled = "#9aa5b1";

  t.primary = "#0f5e9c";
  t.primary_hover = "#0c4f85";
  t.primary_pressed = "#093f6b";
  t.on_primary = "#ffffff";
  t.primary_soft = "#e4eff8";

  t.sidebar = "#0b2f4a";
  t.sidebar_text = "#ffffff";
  t.sidebar_muted = "#9fb7cb";
  t.sidebar_active = "#17486c";

  t.info_text = "#12497f";
  t.info_bg = "#e7f1fb";
  t.info_accent = "#3b82c4";
  t.caution_text = "#7a4300";
  t.caution_bg = "#fff4de";
  t.caution_accent = "#e09b1b";
  t.danger_text = "#9c1c14";
  t.danger_bg = "#fdebe9";
  t.danger_accent = "#d2372c";
  t.success_text = "#14603a";
  t.success_bg = "#e6f5ec";
  t.success_accent = "#2f9e60";

  t.line_first = "#0f5e9c";
  t.line_second = "#6a4fb5";
  t.line_allergy = "#b76a00";
  return t;
}

Theme darkTheme() {
  Theme t;
  t.dark = true;
  t.window = "#12171c";
  t.surface = "#1b222a";
  t.surface_alt = "#242d37";
  t.border = "#33404d";
  t.text = "#e7ecf1";
  t.muted = "#9dabb9";
  t.disabled = "#62707e";

  t.primary = "#4a9fe0";
  t.primary_hover = "#63aee8";
  t.primary_pressed = "#3a8acb";
  t.on_primary = "#06131f";
  t.primary_soft = "#1b3145";

  t.sidebar = "#0b1f30";
  t.sidebar_text = "#ffffff";
  t.sidebar_muted = "#8fa8bd";
  t.sidebar_active = "#173a57";

  t.info_text = "#a9cdf5";
  t.info_bg = "#18293c";
  t.info_accent = "#4a9fe0";
  t.caution_text = "#f7d489";
  t.caution_bg = "#33290f";
  t.caution_accent = "#e0a72e";
  t.danger_text = "#f6a8a1";
  t.danger_bg = "#3a1c1a";
  t.danger_accent = "#e0574c";
  t.success_text = "#9bdcb6";
  t.success_bg = "#16301f";
  t.success_accent = "#3fae72";

  t.line_first = "#4a9fe0";
  t.line_second = "#a48be6";
  t.line_allergy = "#e0a04a";
  return t;
}

Theme& activeTheme() {
  static Theme active = lightTheme();
  return active;
}

QPalette paletteFor(const Theme& t) {
  QPalette palette;
  const QColor window(t.window), surface(t.surface), text(t.text), disabled(t.disabled);
  palette.setColor(QPalette::Window, window);
  palette.setColor(QPalette::WindowText, text);
  palette.setColor(QPalette::Base, surface);
  palette.setColor(QPalette::AlternateBase, QColor(t.surface_alt));
  palette.setColor(QPalette::Text, text);
  palette.setColor(QPalette::Button, surface);
  palette.setColor(QPalette::ButtonText, text);
  palette.setColor(QPalette::ToolTipBase, surface);
  palette.setColor(QPalette::ToolTipText, text);
  palette.setColor(QPalette::PlaceholderText, QColor(t.disabled));
  palette.setColor(QPalette::Highlight, QColor(t.primary));
  palette.setColor(QPalette::HighlightedText, QColor(t.on_primary));
  palette.setColor(QPalette::Link, QColor(t.primary));
  palette.setColor(QPalette::Mid, QColor(t.border));
  palette.setColor(QPalette::Midlight, QColor(t.surface_alt));
  palette.setColor(QPalette::Dark, QColor(t.border));
  palette.setColor(QPalette::Light, surface);
  for (const QPalette::ColorRole role :
       {QPalette::WindowText, QPalette::Text, QPalette::ButtonText}) {
    palette.setColor(QPalette::Disabled, role, disabled);
  }
  return palette;
}

/// Dibuja una flecha en «V» (hacia abajo o hacia arriba) y la guarda en dos
/// resoluciones: «nombre.png» y «nombre@2x.png» (pantallas de alta densidad).
void saveChevron(const QString& folder, const QString& name, const QColor& color, bool up) {
  for (const int scale : {1, 2}) {
    const int size = 10 * scale;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.6 * scale, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    const double s = size;
    QPainterPath path;
    if (up) {
      path.moveTo(s * 0.2, s * 0.65);
      path.lineTo(s * 0.5, s * 0.35);
      path.lineTo(s * 0.8, s * 0.65);
    } else {
      path.moveTo(s * 0.2, s * 0.35);
      path.lineTo(s * 0.5, s * 0.65);
      path.lineTo(s * 0.8, s * 0.35);
    }
    painter.drawPath(path);
    painter.end();
    pixmap.save(folder + "/" + name + (scale == 2 ? "@2x.png" : ".png"));
  }
}

/// Carpeta temporal con las flechas de los campos; vive mientras dure el
/// programa. La hoja de estilo solo admite imágenes desde archivos.
QString arrowFolder(const Theme& t) {
  static QTemporaryDir folder;
  const QString path = folder.path();
  saveChevron(path, "down", QColor(t.muted), false);
  saveChevron(path, "up", QColor(t.muted), true);
  saveChevron(path, "down_off", QColor(t.disabled), false);
  saveChevron(path, "up_off", QColor(t.disabled), true);
  return path;
}

/// Hoja de estilo con marcadores «@nombre», que se sustituyen por los colores.
QString styleSheetFor(const Theme& t) {
  QString css = QStringLiteral(R"css(
    QWizard { background: @window; }
    QDialog { background: @surface; }
    QToolTip { background: @surface; color: @text; border: 1px solid @border; padding: 4px 6px; }

    QLabel { color: @text; }
    QLabel#pageTitle { font-size: 26px; font-weight: 700; color: @text; }
    QLabel#pageSubtitle { font-size: 14px; color: @muted; }
    QLabel#fieldLabel { font-size: 11px; font-weight: 700; color: @muted; }
    QLabel#fieldHint { font-size: 12px; color: @muted; }
    QLabel#cardTitle { font-size: 15px; font-weight: 700; color: @text; }
    QLabel#footnote { font-size: 12px; color: @muted; }

    QFrame#card { background: @window; border: 1px solid @border; border-radius: 10px; }
    QFrame#optionCard { background: @surface; border: 1px solid @border; border-radius: 10px; }
    QFrame#optionCard[selected="true"] { border: 2px solid @primary; background: @primary_soft; }
    QFrame#separator { background: @border; border: none; max-height: 1px; min-height: 1px; }

    QLineEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {
      background: @surface; color: @text; border: 1px solid @border; border-radius: 6px;
      padding: 6px 8px; min-height: 20px;
      selection-background-color: @primary; selection-color: @on_primary;
    }
    QLineEdit:focus, QPlainTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus,
    QComboBox:focus { border: 1px solid @primary; }
    QLineEdit:disabled, QSpinBox:disabled, QDoubleSpinBox:disabled, QComboBox:disabled {
      background: @surface_alt; color: @disabled;
    }
    QPlainTextEdit[readOnly="true"] { background: @surface_alt; color: @muted; }
    QLineEdit#search { padding: 8px 12px; font-size: 14px; border-radius: 8px; }

    QComboBox::drop-down {
      subcontrol-origin: padding; subcontrol-position: center right; width: 26px; border: none;
    }
    QComboBox::down-arrow { image: url(@arrows/down.png); width: 10px; height: 10px; }
    QComboBox::down-arrow:disabled { image: url(@arrows/down_off.png); }
    QSpinBox::up-button, QDoubleSpinBox::up-button {
      subcontrol-origin: border; subcontrol-position: top right; width: 24px; border: none;
      margin-top: 3px;
    }
    QSpinBox::down-button, QDoubleSpinBox::down-button {
      subcontrol-origin: border; subcontrol-position: bottom right; width: 24px; border: none;
      margin-bottom: 3px;
    }
    QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
      image: url(@arrows/up.png); width: 10px; height: 10px;
    }
    QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
      image: url(@arrows/down.png); width: 10px; height: 10px;
    }
    QSpinBox::up-arrow:disabled, QDoubleSpinBox::up-arrow:disabled,
    QSpinBox::up-arrow:off, QDoubleSpinBox::up-arrow:off { image: url(@arrows/up_off.png); }
    QSpinBox::down-arrow:disabled, QDoubleSpinBox::down-arrow:disabled,
    QSpinBox::down-arrow:off, QDoubleSpinBox::down-arrow:off { image: url(@arrows/down_off.png); }
    QComboBox QAbstractItemView {
      background: @surface; color: @text; border: 1px solid @border;
      selection-background-color: @primary; selection-color: @on_primary; outline: 0;
    }

    QRadioButton { color: @text; spacing: 8px; font-weight: 600; }
    QRadioButton:disabled { color: @disabled; }

    QPushButton {
      background: @surface; color: @text; border: 1px solid @border; border-radius: 6px;
      padding: 7px 16px; min-height: 18px;
    }
    QPushButton:hover { background: @surface_alt; }
    QPushButton:pressed { background: @border; }
    QPushButton:disabled { color: @disabled; background: @surface_alt; border-color: @border; }
    QPushButton:default, QPushButton#primaryButton {
      background: @primary; color: @on_primary; border: 1px solid @primary;
    }
    QPushButton:default:hover, QPushButton#primaryButton:hover {
      background: @primary_hover; border-color: @primary_hover;
    }
    QPushButton:default:pressed, QPushButton#primaryButton:pressed {
      background: @primary_pressed; border-color: @primary_pressed;
    }
    QPushButton:default:disabled, QPushButton#primaryButton:disabled {
      background: @surface_alt; color: @disabled; border-color: @border;
    }

    QTreeWidget {
      background: @surface; color: @text; border: 1px solid @border; border-radius: 10px;
      padding: 6px; outline: 0; font-size: 14px;
    }
    QTreeWidget::item { padding: 6px 10px; border-radius: 6px; }
    QTreeWidget::item:hover { background: @surface_alt; }
    QTreeWidget::item:selected { background: @primary; color: @on_primary; }
    QTreeWidget::branch { background: transparent; }

    QTextBrowser { background: @surface; color: @text; border: none; }

    QScrollBar:vertical { background: transparent; width: 12px; margin: 2px; }
    QScrollBar::handle:vertical { background: @border; border-radius: 4px; min-height: 28px; }
    QScrollBar::handle:vertical:hover { background: @disabled; }
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }
    QScrollBar:horizontal { background: transparent; height: 12px; margin: 2px; }
    QScrollBar::handle:horizontal { background: @border; border-radius: 4px; min-width: 28px; }
    QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }
  )css");

  // Los nombres más largos primero: «@primary_soft» no debe leerse como
  // «@primary» seguido de «_soft».
  const QList<QPair<QString, QString>> tokens = {
      {"@primary_pressed", t.primary_pressed}, {"@primary_hover", t.primary_hover},
      {"@primary_soft", t.primary_soft},       {"@on_primary", t.on_primary},
      {"@surface_alt", t.surface_alt},         {"@disabled", t.disabled},
      {"@primary", t.primary},                 {"@surface", t.surface},
      {"@window", t.window},                   {"@border", t.border},
      {"@muted", t.muted},                     {"@text", t.text},
  };
  for (const auto& [name, value] : tokens) {
    css.replace(name, value);
  }
  css.replace("@arrows", arrowFolder(t));
  return css;
}

}  // namespace

ThemeChoice themeChoiceFromText(const QString& text) {
  const QString value = text.trimmed().toLower();
  if (value == "claro") {
    return ThemeChoice::Light;
  }
  if (value == "oscuro") {
    return ThemeChoice::Dark;
  }
  return ThemeChoice::System;
}

void applyTheme(QApplication& app, ThemeChoice choice) {
  bool dark = choice == ThemeChoice::Dark;
  if (choice == ThemeChoice::System) {
    dark = QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
  }
  activeTheme() = dark ? darkTheme() : lightTheme();

  QApplication::setStyle(QStyleFactory::create("Fusion"));
  QApplication::setPalette(paletteFor(theme()));

  // Helvetica en todos los textos. Donde no está instalada (Windows no la
  // trae y su licencia no permite distribuirla) se usa la más parecida:
  // Arial en Windows, Nimbus Sans o Liberation Sans en Linux.
  // Se pide solo la primera que exista: nombrar familias ausentes hace que Qt
  // recorra todos los alias de fuentes al arrancar.
  QFont text_font;
  const QStringList installed = QFontDatabase::families();
  for (const char* family :
       {"Helvetica Neue", "Helvetica", "Arial", "Nimbus Sans", "Liberation Sans"}) {
    if (installed.contains(QString::fromLatin1(family))) {
      text_font.setFamily(QString::fromLatin1(family));
      break;
    }
  }
  text_font.setStyleHint(QFont::Helvetica);
  text_font.setPixelSize(14);
  QApplication::setFont(text_font);

  app.setStyleSheet(styleSheetFor(theme()));
  app.setWindowIcon(makeAppIcon());
}

const Theme& theme() {
  return activeTheme();
}

QIcon makeAppIcon() {
  QIcon icon;
  for (const int size : {16, 32, 64, 128, 256}) {
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    const double s = size;

    // Fondo: cuadrado redondeado con el color de marca.
    QPainterPath background;
    background.addRoundedRect(QRectF(0, 0, s, s), s * 0.22, s * 0.22);
    painter.fillPath(background, QColor("#0f5e9c"));

    // Cápsula en diagonal: mitad blanca, mitad celeste.
    painter.translate(s / 2, s / 2);
    painter.rotate(-40);
    const QRectF capsule(-s * 0.34, -s * 0.15, s * 0.68, s * 0.30);
    QPainterPath shape;
    shape.addRoundedRect(capsule, capsule.height() / 2, capsule.height() / 2);
    painter.setClipPath(shape);
    painter.fillRect(QRectF(capsule.left(), capsule.top(), capsule.width() / 2, capsule.height()),
                     QColor("#ffffff"));
    painter.fillRect(QRectF(0, capsule.top(), capsule.width() / 2, capsule.height()),
                     QColor("#8fd0f5"));
    painter.end();
    icon.addPixmap(pixmap);
  }
  return icon;
}

}  // namespace calc::gui
