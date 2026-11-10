/// @file main.cpp
/// @brief Punto de entrada de la interfaz gráfica.
///
/// Opciones:
///   --kb <archivo>        usar esta base de conocimiento
///   --capturas <carpeta>  recorrer el asistente, guardar una imagen de cada
///                         paso y salir (para el manual de usuario)

#include <QApplication>
#include <QCommandLineParser>
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
  parser.addOption(kb_option);
  parser.addOption(tour_option);
  parser.process(app);

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
