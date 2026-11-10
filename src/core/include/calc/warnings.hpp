/// @file warnings.hpp
/// @brief Advertencias que acompañan a un cálculo.
///
/// El motor nunca "falla en silencio": cualquier condición que el usuario deba
/// conocer (dato no verificado, tope aplicado, peso poco plausible...) se
/// devuelve como una advertencia con un código estable. Las interfaces deciden
/// cómo mostrarla; las pruebas comprueban los códigos, no los textos.
#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace calc {

/// Gravedad de una advertencia.
enum class Severity {
  Info,     ///< Informativa; no cambia la conducta.
  Caution,  ///< El usuario debe revisar antes de usar el resultado.
  Blocking  ///< El resultado no debe usarse (o no se calculó).
};

struct Warning {
  Severity severity = Severity::Info;
  std::string code;     ///< Identificador estable en MAYUSCULAS_CON_GUION_BAJO.
  std::string message;  ///< Texto para el usuario, en español.
};

/// Indica si alguna advertencia es bloqueante.
[[nodiscard]] inline bool hasBlocking(const std::vector<Warning>& warnings) {
  return std::any_of(warnings.begin(), warnings.end(),
                     [](const Warning& w) { return w.severity == Severity::Blocking; });
}

/// Indica si existe una advertencia con el código dado.
[[nodiscard]] inline bool hasCode(const std::vector<Warning>& warnings, const std::string& code) {
  return std::any_of(warnings.begin(), warnings.end(),
                     [&](const Warning& w) { return w.code == code; });
}

}  // namespace calc
