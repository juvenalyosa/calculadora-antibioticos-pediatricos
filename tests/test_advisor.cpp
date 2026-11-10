// Comprueba el contrato del punto de extensión (advisor.hpp) con módulos
// ficticios: sirven de plantilla para un futuro módulo real (p. ej. de IA).

#include <doctest/doctest.h>

#include <memory>
#include <stdexcept>

#include "calc/advisor.hpp"
#include "support/test_kb.hpp"

using namespace calc;

namespace {

/// Módulo de ejemplo: comenta cuántas líneas de manejo tiene el plan.
class LineCounterAdvisor final : public Advisor {
 public:
  [[nodiscard]] std::string id() const override { return "contador"; }
  [[nodiscard]] std::vector<Annotation> review(const AdvisorContext& context) const override {
    Annotation annotation;
    annotation.advisor_id = "intento-de-suplantar";  // el registro lo sobrescribe
    annotation.message = "Líneas de manejo: " + std::to_string(context.plan.lines.size());
    return {annotation};
  }
};

/// Módulo defectuoso: siempre falla.
class BrokenAdvisor final : public Advisor {
 public:
  [[nodiscard]] std::string id() const override { return "defectuoso"; }
  [[nodiscard]] std::vector<Annotation> review(const AdvisorContext&) const override {
    throw std::runtime_error("sin conexión");
  }
};

}  // namespace

TEST_CASE("módulos de apoyo: observan el plan y quedan atribuidos") {
  const KnowledgeBase kb = testing::loadTestKb();
  const CalculationRequest request{testing::infantCase(), RenalFunction::assumedNormal(),
                                   "enfermedad_x"};
  const TreatmentPlan plan = DoseEngine(kb).plan(request);

  AdvisorRegistry registry;
  CHECK(registry.reviewAll({request, plan}).empty());  // sin módulos no hay observaciones

  registry.add(std::make_unique<LineCounterAdvisor>());
  registry.add(nullptr);  // se ignora
  CHECK(registry.size() == 1);

  const auto annotations = registry.reviewAll({request, plan});
  REQUIRE(annotations.size() == 1);
  CHECK(annotations[0].advisor_id == "contador");
  CHECK(annotations[0].message == "Líneas de manejo: 3");
}

TEST_CASE("módulos de apoyo: un módulo que falla no afecta a los demás ni al cálculo") {
  const KnowledgeBase kb = testing::loadTestKb();
  const CalculationRequest request{testing::infantCase(), RenalFunction::assumedNormal(),
                                   "enfermedad_x"};
  const TreatmentPlan plan = DoseEngine(kb).plan(request);

  AdvisorRegistry registry;
  registry.add(std::make_unique<BrokenAdvisor>());
  registry.add(std::make_unique<LineCounterAdvisor>());

  std::vector<Annotation> annotations;
  CHECK_NOTHROW(annotations = registry.reviewAll({request, plan}));
  REQUIRE(annotations.size() == 2);
  CHECK(annotations[0].advisor_id == "defectuoso");
  CHECK(annotations[0].severity == Severity::Info);
  CHECK(annotations[0].message.find("sin conexión") != std::string::npos);
  CHECK(annotations[1].advisor_id == "contador");
}
