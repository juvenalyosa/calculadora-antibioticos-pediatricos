/// @file error_report.hpp
/// @brief Reporte de errores por parte del usuario (fase beta).
///
/// La metodología del proyecto exige que en CADA paso el usuario pueda
/// reportar un posible error mediante un formato semiautomatizado que guarde
/// los parámetros introducidos. `makeErrorReport` arma ese formato a partir
/// del estado del cálculo y `ReportSink` decide a dónde se envía.
///
/// Privacidad: el reporte solo contiene las variables del cálculo (edad, peso,
/// talla, sexo, función renal, enfermedad). No hay campo para identificar al
/// paciente.
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "calc/dose_engine.hpp"

namespace calc {

/// Tipo de problema reportado.
enum class ReportCategory {
  CalculationError,  ///< La dosis calculada parece incorrecta.
  ClinicalContent,   ///< El contenido de la guía parece incorrecto o desactualizado.
  Usability,         ///< Problema de la interfaz o de su claridad.
  Other
};

[[nodiscard]] std::string_view toString(ReportCategory category) noexcept;

struct ErrorReport {
  std::string timestamp_utc;  ///< ISO 8601, p. ej. "2026-11-10T15:04:05Z".
  std::string app_version;
  std::string kb_version;
  std::string step;  ///< Paso del asistente donde se reportó (p. ej. "resultado").
  ReportCategory category = ReportCategory::Other;
  std::string comment;  ///< Texto libre del usuario.
  /// Parámetros introducidos, como pares clave → valor en texto.
  std::map<std::string, std::string> context;
};

/// Construye un reporte capturando automáticamente fecha, versiones y los
/// parámetros del cálculo en curso.
[[nodiscard]] ErrorReport makeErrorReport(const std::string& step, ReportCategory category,
                                          const std::string& comment,
                                          const CalculationRequest& request,
                                          const std::string& kb_version);

/// Destino de los reportes. Implementaciones previstas: archivo local (hoy),
/// envío HTTP a la hoja de errores y correcciones (fase beta).
class ReportSink {
 public:
  virtual ~ReportSink() = default;
  /// @throws std::runtime_error si el reporte no se pudo entregar.
  virtual void submit(const ErrorReport& report) = 0;
};

/// Guarda cada reporte como una línea JSON (formato JSON Lines) en un archivo
/// local. Sirve sin conexión y como cola de respaldo del envío remoto.
class JsonlFileSink final : public ReportSink {
 public:
  explicit JsonlFileSink(std::filesystem::path path) : path_(std::move(path)) {}
  void submit(const ErrorReport& report) override;

 private:
  std::filesystem::path path_;
};

}  // namespace calc
