/// @file kb_loader.hpp
/// @brief Lectura y validación de la base de conocimiento desde JSON.
///
/// El cargador es la ÚNICA puerta de entrada de datos clínicos al programa.
/// Si devuelve un `KnowledgeBase`, el resto del código puede confiar en que:
///   - todos los identificadores son únicos y las referencias existen;
///   - las dosis cumplen 0 < mínimo <= usual <= máximo;
///   - las bandas renales no se solapan;
///   - lo marcado como "verificado" tiene dos verificadores distintos.
///
/// Otras fuentes futuras (SQLite, descarga remota) deben producir el mismo
/// `KnowledgeBase` pasando por estas validaciones.
#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "calc/knowledge_base.hpp"

namespace calc {

/// Versión de la forma del JSON que este programa sabe leer. Debe aumentarse
/// cuando un cambio en la estructura deje de ser compatible hacia atrás.
inline constexpr int kSupportedSchemaVersion = 1;

/// Error al cargar la base. Reúne TODOS los problemas encontrados, no solo el
/// primero, para que quien corrige los datos lo haga en una sola pasada.
class KnowledgeBaseError : public std::runtime_error {
 public:
  explicit KnowledgeBaseError(std::vector<std::string> issues);
  [[nodiscard]] const std::vector<std::string>& issues() const noexcept { return issues_; }

 private:
  std::vector<std::string> issues_;
};

/// @throws KnowledgeBaseError si el texto no es JSON válido o incumple alguna regla.
[[nodiscard]] KnowledgeBase loadKnowledgeBaseFromString(std::string_view json_text);

/// @throws KnowledgeBaseError si el archivo no se puede leer o no es válido.
[[nodiscard]] KnowledgeBase loadKnowledgeBaseFromFile(const std::filesystem::path& path);

}  // namespace calc
