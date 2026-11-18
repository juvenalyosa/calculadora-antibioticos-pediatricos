/// @file knowledge_base.hpp
/// @brief Modelo en memoria de la base de conocimiento clínica.
///
/// Todo el contenido clínico (enfermedades, líneas de manejo, dosis, ajustes
/// renales, presentaciones) vive en DATOS, no en código. Este archivo define
/// la forma de esos datos; kb_loader.hpp los lee y valida desde JSON.
///
/// Jerarquía:
///
///     KnowledgeBase
///     ├── Source[]                 guías y referencias citadas
///     ├── Drug[]                   medicamento
///     │   ├── Presentation[]       concentración y forma farmacéutica
///     │   └── RenalAdjustment      ajuste por TFG (propio del medicamento)
///     └── Disease[]                enfermedad
///         └── TreatmentLine[]      primera / segunda / alergia
///             └── TherapyOption[]  alternativa dentro de la línea
///                 └── Regimen[]    1 componente, o varios si es combinación
///
/// La correspondencia entre estos nombres y las claves en español del JSON
/// está en docs/guia_datos.md.
#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace calc {

// --- Enumeraciones -----------------------------------------------------------

/// Unidad en que se expresa el principio activo de un medicamento.
enum class DrugUnit { Milligram, InternationalUnit };

/// Vía de administración.
enum class Route { Oral, Intravenous, Intramuscular, Topical };

/// Cómo está expresada la dosis de un esquema.
enum class DoseBasis {
  PerKgPerDay,   ///< mg/kg/día, repartido según el intervalo.
  PerKgPerDose,  ///< mg/kg por cada administración.
  Fixed,         ///< Dosis fija (p. ej. por rango de peso).
  NoCalculation  ///< Indicación sin cálculo (p. ej. tópico); solo texto.
};

enum class PresentationKind {
  Liquid,  ///< Se mide en mL (suspensiones, viales reconstituidos).
  Solid    ///< Se cuenta en unidades (tabletas, cápsulas, viales enteros).
};

enum class TreatmentLineKind { First, Second, Allergy };

/// Estado de revisión clínica de un dato.
enum class ReviewStatus {
  Sample,   ///< Dato de muestra para desarrollo. NO usar clínicamente.
  Draft,    ///< Digitado por un asistente, pendiente de verificación.
  Verified  ///< Verificado de forma independiente por dos personas.
};

enum class RenalAdjustmentKind {
  NotRequired,  ///< La referencia indica que no requiere ajuste.
  Banded,       ///< Ajuste según bandas de TFG.
  NoData        ///< Aún no se ha compilado la información.
};

// --- Entidades ---------------------------------------------------------------

/// Guía de práctica clínica o referencia de donde sale un dato.
struct Source {
  std::string id;
  std::string title;
  std::string organization;
  int year = 0;
  std::string url;
};

/// Presentación comercial de un medicamento.
struct Presentation {
  std::string id;
  std::string description;  ///< P. ej. "Suspensión 250 mg/5 mL".
  Route route = Route::Oral;
  PresentationKind kind = PresentationKind::Liquid;
  /// Cantidad de principio activo por unidad de dispensación: por mL en
  /// líquidos, por tableta/cápsula/vial en sólidos. En la unidad del medicamento.
  double amount_per_unit = 0.0;
  /// Nombre de la unidad de dispensación: "mL", "tableta", "cápsula", "vial".
  std::string dispensing_unit;
  /// Mínima fracción medible o partible (0,5 mL; 0,5 tableta; 1 cápsula).
  double rounding_step = 1.0;
};

/// Banda de ajuste renal: aplica cuando gfr_min <= TFG < gfr_max.
struct RenalBand {
  double gfr_min = 0.0;
  double gfr_max = 0.0;
  /// Fracción de la dosis por toma que se conserva (1 = sin cambio).
  double dose_factor = 1.0;
  /// Intervalo mínimo entre tomas. El motor usa el mayor entre este y el del
  /// esquema, de modo que un ajuste renal nunca aumenta la frecuencia.
  std::optional<int> min_interval_hours;
  /// Dosis de reemplazo por toma, por kilogramo de peso: sustituye a la del
  /// esquema (p. ej. «15 mg/kg cada 12 h»). Vacío = se conserva la del esquema.
  std::optional<double> replacement_dose_per_kg;
  /// Tope por toma propio de la banda; se aplica después del ajuste.
  std::optional<double> max_per_dose;
  /// El medicamento debe evitarse en esta banda.
  bool avoid = false;
  std::string note;
};

struct RenalAdjustment {
  RenalAdjustmentKind kind = RenalAdjustmentKind::NoData;
  std::vector<RenalBand> bands;  ///< Solo si kind == Banded.
  std::string note;
  std::string source_id;  ///< Puede estar vacío mientras no haya dato.
  ReviewStatus status = ReviewStatus::Sample;

  /// Banda que contiene la TFG dada, o nullptr si ninguna aplica.
  [[nodiscard]] const RenalBand* findBand(double gfr) const noexcept;
};

/// Dosis máxima del medicamento por vía, con independencia de la indicación
/// (en la práctica, la dosis máxima del adulto). Se aplica a los esquemas que
/// no traen su propio tope de ese tipo: así una dosis por kilogramo no crece
/// sin límite en los pacientes de mayor peso.
struct DrugCap {
  Route route = Route::Oral;
  std::optional<double> max_per_dose;
  std::optional<double> max_per_day;
  std::string source_id;
  std::string page;
  std::string note;
};

struct Drug {
  std::string id;
  std::string name;
  DrugUnit unit = DrugUnit::Milligram;
  std::vector<Presentation> presentations;
  RenalAdjustment renal;
  std::vector<DrugCap> caps;  ///< A lo sumo uno por vía.

  /// Tope del medicamento para una vía, si existe.
  [[nodiscard]] const DrugCap* capFor(Route route) const noexcept {
    for (const DrugCap& cap : caps) {
      if (cap.route == route) {
        return &cap;
      }
    }
    return nullptr;
  }
};

/// Un medicamento con su dosis dentro de una opción terapéutica.
///
/// Todas las cantidades están en la unidad del medicamento (`Drug::unit`).
/// Los rangos de edad y peso son semiabiertos: [mínimo, máximo).
struct Regimen {
  std::string drug_id;
  Route route = Route::Oral;
  DoseBasis basis = DoseBasis::PerKgPerDay;

  double dose_min = 0.0;    ///< Límite inferior del rango de la guía.
  double dose_usual = 0.0;  ///< Valor con el que calcula el motor.
  double dose_max = 0.0;    ///< Límite superior del rango de la guía.

  /// Horas entre tomas. Vacío = dosis única.
  std::optional<int> interval_hours;

  std::optional<double> max_per_dose;  ///< Tope por toma.
  std::optional<double> max_per_day;   ///< Tope diario.

  std::optional<int> duration_min_days;
  std::optional<int> duration_max_days;

  std::optional<int> age_min_months;
  std::optional<int> age_max_months;
  std::optional<double> weight_min_kg;
  std::optional<double> weight_max_kg;

  std::string note;
  std::string source_id;
  std::string page;  ///< Página o tabla dentro de la fuente.
  ReviewStatus status = ReviewStatus::Sample;
  std::vector<std::string> verified_by;  ///< Dos personas si status == Verified.
};

/// Alternativa dentro de una línea. Varios componentes = terapia combinada.
struct TherapyOption {
  int number = 0;  ///< Número estable dentro de la línea (lo usa la matriz alfa).
  std::string description;
  std::vector<Regimen> components;
};

struct TreatmentLine {
  TreatmentLineKind kind = TreatmentLineKind::First;
  std::vector<TherapyOption> options;
};

struct Disease {
  std::string id;
  std::string name;
  std::string group;  ///< Agrupación para la interfaz (p. ej. "Respiratorias").
  /// false = entidad en la que NO se indican antibióticos (p. ej. bronquiolitis).
  bool antibiotic_indicated = true;
  std::string message;  ///< Texto al usuario; obligatorio si no hay antibiótico.
  std::string source_id;
  std::vector<TreatmentLine> lines;
};

/// Se lanza cuando se pide por identificador algo que no existe en la base.
class NotFoundError : public std::out_of_range {
 public:
  using std::out_of_range::out_of_range;
};

/// Base de conocimiento completa, ya validada por el cargador.
struct KnowledgeBase {
  int schema_version = 0;  ///< Versión de la FORMA de los datos.
  std::string version;     ///< Versión del CONTENIDO clínico.
  std::string date;        ///< Fecha de publicación (AAAA-MM-DD).
  std::vector<Source> sources;
  std::vector<Drug> drugs;
  std::vector<Disease> diseases;

  // Búsquedas que devuelven nullptr si el identificador no existe.
  [[nodiscard]] const Source* findSource(std::string_view id) const noexcept;
  [[nodiscard]] const Drug* findDrug(std::string_view id) const noexcept;
  [[nodiscard]] const Disease* findDisease(std::string_view id) const noexcept;

  // Variantes que lanzan NotFoundError.
  [[nodiscard]] const Drug& drug(std::string_view id) const;
  [[nodiscard]] const Disease& disease(std::string_view id) const;
};

// --- Textos visibles ---------------------------------------------------------

[[nodiscard]] std::string_view symbol(DrugUnit unit) noexcept;            ///< "mg", "UI".
[[nodiscard]] std::string_view label(Route route) noexcept;               ///< "vía oral"...
[[nodiscard]] std::string_view label(TreatmentLineKind kind) noexcept;    ///< "Primera línea"...
[[nodiscard]] std::string_view label(ReviewStatus status) noexcept;       ///< "verificado"...

}  // namespace calc
