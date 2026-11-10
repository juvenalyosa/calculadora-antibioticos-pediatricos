#include "app_paths.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStringList>

namespace calc::gui {

namespace {
constexpr const char* kKbFileName = "base_conocimiento.json";
}

QString locateKnowledgeBase(const QString& explicit_path) {
  if (!explicit_path.isEmpty()) {
    // Si el usuario pidió un archivo concreto no se busca en otro lado: un
    // error aquí debe verse, no ocultarse cargando otra base.
    return QFileInfo::exists(explicit_path) ? explicit_path : QString();
  }

  const QString app_dir = QCoreApplication::applicationDirPath();
  const QStringList candidates = {
      qEnvironmentVariable("CALC_KB_FILE"),
      app_dir + "/../Resources/" + kKbFileName,  // paquete .app de macOS
      app_dir + "/data/kb/" + kKbFileName,       // instalación en Windows y Linux
      QStringLiteral(CALC_DEV_KB_FILE),          // árbol de fuentes
  };
  for (const QString& candidate : candidates) {
    if (!candidate.isEmpty() && QFileInfo::exists(candidate)) {
      return QDir::cleanPath(candidate);
    }
  }
  return QString();
}

QString reportFilePath() {
  const QString folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(folder);
  return folder + "/reportes_de_error.jsonl";
}

}  // namespace calc::gui
