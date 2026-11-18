/// @file main.cpp
/// @brief Punto de entrada de la interfaz gráfica.
///
/// Opciones:
///   --kb <archivo>        usar esta base de conocimiento
///   --capturas <carpeta>  recorrer el asistente, guardar una imagen de cada
///                         paso y salir (para el manual de usuario)
///   --tema <nombre>       claro, oscuro o sistema (por defecto, el del sistema)
///   --exportar-icono <carpeta>  guardar el icono en PNG de varios tamaños y salir

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

#include "app_paths.hpp"
#include "calc/advisor.hpp"
#include "calc/error_report.hpp"
#include "calc/kb_loader.hpp"
#include "calc/version.hpp"
#include "calculator_wizard.hpp"
#include "html.hpp"
#include "screenshot_tour.hpp"
#include "theme.hpp"

int main(int argc, char** argv) {
  using namespace calc;
  using namespace calc::gui;

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("UMNG");
  QCoreApplication::setOrganizationDomain("unimilitar.edu.co");
  QCoreApplication::setApplicationName("CalculadoraAntibioticos");
  QCoreApplication::setApplicationVersion(toQString(kAppVersion));

  // Español de Colombia: coma decimal en los campos numéricos.
  QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Colombia));
  // Textos propios de Qt (diálogos estándar) en español, si están instalados.
  QTranslator qt_translator;
  if (qt_translator.load("qtbase_es", QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
    QCoreApplication::installTranslator(&qt_translator);
  }

  QCommandLineParser parser;
  parser.setApplicationDescription(toQString(kAppName));
  parser.addHelpOption();
  parser.addVersionOption();
  const QCommandLineOption kb_option("kb", "Archivo de la base de conocimiento.", "archivo");
  const QCommandLineOption tour_option(
      "capturas", "Guardar una imagen de cada paso en la carpeta y salir.", "carpeta");
  const QCommandLineOption theme_option(
      "tema", "Aspecto: claro, oscuro o sistema (por defecto).", "nombre", "sistema");
  parser.addOption(kb_option);
  parser.addOption(tour_option);
  const QCommandLineOption icon_option(
      "exportar-icono", "Guardar el icono en varios tamaños (PNG) en la carpeta y salir.", "carpeta");
  parser.addOption(theme_option);
  parser.addOption(icon_option);
  parser.process(app);

  // Colores, tipografía e icono; antes de crear cualquier ventana.
  applyTheme(app, themeChoiceFromText(parser.value(theme_option)));

  // Los iconos de los instaladores (.icns, .ico, .png) salen de este dibujo.
  if (parser.isSet(icon_option)) {
    const QDir folder(parser.value(icon_option));
    bool saved = QDir().mkpath(folder.path());
    for (const int size : {16, 32, 48, 64, 128, 256, 512, 1024}) {
      saved = renderAppIcon(size).save(folder.filePath(QString("icono_%1.png").arg(size))) && saved;
    }
    return saved ? 0 : 1;
  }

  // --- Base de conocimiento: sin ella no hay nada que mostrar ---
  const QString kb_path = locateKnowledgeBase(parser.value(kb_option));
  if (kb_path.isEmpty()) {
    QMessageBox::critical(nullptr, "No se encontró la base de conocimiento",
                          "No se encontró el archivo base_conocimiento.json. "
                          "Reinstale la aplicación o indique el archivo con la opción --kb.");
    return 1;
  }

  KnowledgeBase kb;
  try {
    kb = loadKnowledgeBaseFromFile(kb_path.toStdString());
  } catch (const KnowledgeBaseError& error) {
    QMessageBox box(QMessageBox::Critical, "La base de conocimiento no es válida",
                    "El archivo " + kb_path + " tiene errores y no se puede usar.");
    box.setDetailedText(toQString(error.what()));
    box.exec();
    return 1;
  }

  // --- Servicios que usa la ventana ---
  JsonlFileSink report_sink(reportFilePath().toStdString());
  // Aquí se registran los módulos de apoyo (p. ej. de IA) cuando existan:
  //   advisors.add(std::make_unique<MiModulo>());
  AdvisorRegistry advisors;

  CalculatorWizard wizard(kb, report_sink, advisors);

  if (parser.isSet(tour_option)) {
    return runScreenshotTour(wizard, report_sink, parser.value(tour_option)) ? 0 : 1;
  }

  wizard.show();
  return QApplication::exec();
}
