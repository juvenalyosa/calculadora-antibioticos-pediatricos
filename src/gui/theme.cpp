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

/// Colores de partida de una gama; el resto del tema se deriva de ellos.
struct Gamut {
  const char* window;        // fondo claro
  const char* surface_alt;   // fondo de campos
  const char* border;
  const char* text;          // tono oscuro: texto y banda del encabezado
  const char* muted;
  const char* disabled;
  const char* primary;       // acciones y cifras destacadas
  const char* primary_hover;
  const char* primary_pressed;
  const char* primary_soft;
  const char* band_to;       // extremo derecho de la banda (degradado sutil)
  const char* band_muted;    // texto secundario sobre la banda
  const char* accent;        // detalle fino (línea y aro del paso actual)
  const char* line_second;
};

// Gama del proyecto: azul marino y marfil, con un detalle fino en dorado. El
// tono oscuro da el texto y la banda del encabezado; el marfil, el fondo. Los
// avisos (ámbar, rojo, verde) quedan fuera de la gama a propósito: una
// advertencia debe distinguirse.
constexpr Gamut kGamut = {"#f6f3ee", "#faf8f4", "#e4ded3", "#14213d", "#5a6478", "#a9afba", "#1f4e79",
                          "#1a426a", "#153757", "#e3eaf2", "#1f3558", "#c5cedd", "#c8a45d", "#6b7a99"};

void setSemanticColors(Theme& t) {
  if (t.dark) {
    t.caution_text = "#f7d489"; t.caution_bg = "#3a3013"; t.caution_accent = "#e0a72e";
    t.danger_text = "#f6a8a1";  t.danger_bg = "#40211e";  t.danger_accent = "#e0574c";
    t.success_text = "#9bdcb6"; t.success_bg = "#173a2a"; t.success_accent = "#3fae72";
    t.line_allergy = "#e0a04a";
  } else {
    t.caution_text = "#7a4a00"; t.caution_bg = "#fff3d6"; t.caution_accent = "#e9a23b";
    t.danger_text = "#9a2a20";  t.danger_bg = "#fde8e4";  t.danger_accent = "#d6513f";
    t.success_text = "#1e6b4a"; t.success_bg = "#e0f3e8"; t.success_accent = "#3ba776";
    t.line_allergy = "#a66a1f";
  }
}

Theme lightTheme() {
  const Gamut& g = kGamut;
  Theme t;
  t.dark = false;
  t.window = g.window;
  t.surface = "#ffffff";
  t.surface_alt = g.surface_alt;
  t.border = g.border;
  t.text = g.text;
  t.muted = g.muted;
  t.disabled = g.disabled;
  t.primary = g.primary;
  t.primary_hover = g.primary_hover;
  t.primary_pressed = g.primary_pressed;
  t.on_primary = "#ffffff";
  t.primary_soft = g.primary_soft;
  t.sidebar = g.text;
  t.sidebar_text = "#ffffff";
  t.sidebar_muted = g.band_muted;
  t.sidebar_active = g.band_to;
  t.accent = g.accent;
  t.info_text = g.primary;
  t.info_bg = g.primary_soft;
  t.info_accent = g.primary;
  t.line_first = g.primary;
  t.line_second = g.line_second;
  setSemanticColors(t);
  return t;
}

/// El tema oscuro se deriva de la misma gama: el tono oscuro pasa a ser el
/// fondo y el color de marca se aclara para conservar el contraste.
Theme darkTheme() {
  const Gamut& g = kGamut;
  const QColor deep(g.text);
  const QColor brand(g.primary);
  Theme t;
  t.dark = true;
  t.window = deep.darker(135).name();
  t.surface = deep.name();
  t.surface_alt = deep.lighter(130).name();
  t.border = deep.lighter(170).name();
  t.text = g.window;
  t.muted = g.band_muted;
  t.disabled = QColor(g.band_muted).darker(170).name();
  t.primary = brand.lighter(175).name();
  t.primary_hover = brand.lighter(195).name();
  t.primary_pressed = brand.lighter(155).name();
  t.on_primary = deep.darker(160).name();
  t.primary_soft = deep.lighter(150).name();
  t.sidebar = deep.lighter(120).name();
  t.sidebar_text = "#ffffff";
  t.sidebar_muted = g.band_muted;
  t.sidebar_active = QColor(g.band_to).lighter(120).name();
  t.accent = g.accent;
  t.info_text = brand.lighter(210).name();
  t.info_bg = deep.lighter(150).name();
  t.info_accent = t.primary;
  t.line_first = t.primary;
  t.line_second = g.band_muted;
  setSemanticColors(t);
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
    QWizard, QDialog { background: @window; }
    QToolTip { background: @surface; color: @text; border: 1px solid @border; padding: 4px 6px; }

    QLabel { color: @text; }
    QLabel#pageTitle { font-size: 26px; font-weight: 700; color: @text; }
    QLabel#bigValue { font-size: 22px; font-weight: 700; color: @primary; }
    QLabel#pageSubtitle { font-size: 14px; color: @muted; }
    QLabel#fieldLabel { font-size: 11px; font-weight: 700; color: @muted; }
    QLabel#fieldHint { font-size: 12px; color: @muted; }
    QLabel#cardTitle { font-size: 15px; font-weight: 700; color: @text; }
    QLabel#footnote { font-size: 12px; color: @muted; }

    QFrame#card { background: @surface; border: none; border-radius: 18px; }
    QFrame#optionCard { background: @surface; border: 2px solid @surface; border-radius: 18px; }
    QFrame#optionCard[selected="true"] { border: 2px solid @primary; background: @surface; }
    QFrame#separator { background: @border; border: none; max-height: 1px; min-height: 1px; }

    QLineEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {
      background: @surface_alt; color: @text; border: 1px solid @border; border-radius: 10px;
      padding: 8px 10px; min-height: 22px;
      selection-background-color: @primary; selection-color: @on_primary;
    }
    QLineEdit:focus, QPlainTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus,
    QComboBox:focus { border: 1px solid @primary; }
    QLineEdit:disabled, QSpinBox:disabled, QDoubleSpinBox:disabled, QComboBox:disabled {
      background: @surface_alt; color: @disabled;
    }
    QPlainTextEdit[readOnly="true"] { background: @surface_alt; color: @muted; }
    QLineEdit#search { background: @surface; padding: 10px 16px; font-size: 15px; border-radius: 21px; }

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
      background: @surface; color: @text; border: 1px solid @border; border-radius: 19px;
      padding: 9px 22px; min-height: 18px;
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
      background: @surface; color: @text; border: none; border-radius: 18px;
      padding: 10px; outline: 0; font-size: 14px;
    }
    QTreeWidget::item { padding: 7px 12px; border-radius: 12px; }
    QTreeWidget::item:hover { background: @surface_alt; }
    QTreeWidget::item:selected { background: @primary; color: @on_primary; }
    QTreeWidget::branch { background: transparent; }

    QTextBrowser { background: @surface; color: @text; border: none; border-radius: 18px; padding: 14px 18px; }
    QLabel#messages { background: @surface; border-radius: 18px; padding: 10px 14px; }
    QLabel#notice { background: @primary_soft; color: @text; border-radius: 14px; padding: 12px 16px; font-size: 13px; }

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

QPixmap renderAppIcon(int size) {
  QPixmap pixmap(size, size);
  pixmap.fill(Qt::transparent);
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  const double s = size;

  // Fondo: cuadrado redondeado con el color de marca.
  QPainterPath background;
  background.addRoundedRect(QRectF(0, 0, s, s), s * 0.22, s * 0.22);
  painter.fillPath(background, QColor(theme().dark ? theme().primary_pressed : theme().primary));

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
                   QColor(theme().dark ? theme().surface_alt : theme().primary_soft));
  painter.end();
  return pixmap;
}

QIcon makeAppIcon() {
  QIcon icon;
  for (const int size : {16, 32, 64, 128, 256}) {
    icon.addPixmap(renderAppIcon(size));
  }
  return icon;
}

}  // namespace calc::gui
