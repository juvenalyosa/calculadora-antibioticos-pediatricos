#include "calc/knowledge_base.hpp"

#include <algorithm>

namespace calc {

namespace {

/// Búsqueda lineal por `id`. Las colecciones tienen decenas de elementos, así
/// que no se justifica un índice.
template <typename T>
const T* findById(const std::vector<T>& items, std::string_view id) noexcept {
  const auto it =
      std::find_if(items.begin(), items.end(), [&](const T& item) { return item.id == id; });
  return it == items.end() ? nullptr : &*it;
}

}  // namespace

const RenalBand* RenalAdjustment::findBand(double gfr) const noexcept {
  for (const RenalBand& band : bands) {
    if (gfr >= band.gfr_min && gfr < band.gfr_max) {
      return &band;
    }
  }
  return nullptr;
}

const Source* KnowledgeBase::findSource(std::string_view id) const noexcept {
  return findById(sources, id);
}

const Drug* KnowledgeBase::findDrug(std::string_view id) const noexcept {
  return findById(drugs, id);
}

const Disease* KnowledgeBase::findDisease(std::string_view id) const noexcept {
  return findById(diseases, id);
}

const Drug& KnowledgeBase::drug(std::string_view id) const {
  if (const Drug* found = findDrug(id)) {
    return *found;
  }
  throw NotFoundError("Medicamento no encontrado en la base de conocimiento: " + std::string(id));
}

const Disease& KnowledgeBase::disease(std::string_view id) const {
  if (const Disease* found = findDisease(id)) {
    return *found;
  }
  throw NotFoundError("Enfermedad no encontrada en la base de conocimiento: " + std::string(id));
}

std::string_view symbol(DrugUnit unit) noexcept {
  switch (unit) {
    case DrugUnit::Milligram:
      return "mg";
    case DrugUnit::InternationalUnit:
      return "UI";
  }
  return "";
}

std::string_view label(Route route) noexcept {
  switch (route) {
    case Route::Oral:
      return "vía oral";
    case Route::Intravenous:
      return "vía intravenosa";
    case Route::Intramuscular:
      return "vía intramuscular";
    case Route::Topical:
      return "vía tópica";
  }
  return "";
}

std::string_view label(TreatmentLineKind kind) noexcept {
  switch (kind) {
    case TreatmentLineKind::First:
      return "Primera línea";
    case TreatmentLineKind::Second:
      return "Segunda línea";
    case TreatmentLineKind::Allergy:
      return "Alternativa en alergia";
  }
  return "";
}

std::string_view label(ReviewStatus status) noexcept {
  switch (status) {
    case ReviewStatus::Sample:
      return "muestra";
    case ReviewStatus::Draft:
      return "borrador";
    case ReviewStatus::Verified:
      return "verificado";
  }
  return "";
}

}  // namespace calc
