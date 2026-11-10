#include "calc/advisor.hpp"

#include <exception>
#include <utility>

namespace calc {

void AdvisorRegistry::add(std::unique_ptr<Advisor> advisor) {
  if (advisor) {
    advisors_.push_back(std::move(advisor));
  }
}

std::vector<Annotation> AdvisorRegistry::reviewAll(const AdvisorContext& context) const {
  std::vector<Annotation> annotations;
  for (const auto& advisor : advisors_) {
    std::string id = "desconocido";
    try {
      id = advisor->id();
      for (Annotation& annotation : advisor->review(context)) {
        annotation.advisor_id = id;  // La atribución la fija el registro.
        annotations.push_back(std::move(annotation));
      }
    } catch (const std::exception& error) {
      // Un módulo de apoyo nunca debe impedir que el usuario vea el cálculo.
      annotations.push_back({id, Severity::Info,
                             "El módulo de apoyo «" + id + "» falló y se omitió: " + error.what()});
    } catch (...) {
      annotations.push_back(
          {id, Severity::Info, "El módulo de apoyo «" + id + "» falló y se omitió."});
    }
  }
  return annotations;
}

}  // namespace calc
