#include "calc/kb_loader.hpp"

#include <algorithm>
#include <fstream>
#include <initializer_list>
#include <set>
#include <sstream>
#include <utility>

#include <nlohmann/json.hpp>

namespace calc {

namespace {

using nlohmann::json;

std::string buildMessage(const std::vector<std::string>& issues) {
  std::string message = "La base de conocimiento no es válida (" + std::to_string(issues.size()) +
                        (issues.size() == 1 ? " problema):" : " problemas):");
  for (const std::string& issue : issues) {
    message += "\n  - " + issue;
  }
  return message;
}

/// Lee el JSON y acumula todos los problemas encontrados.
///
/// Trabaja en dos fases:
///   1. Lectura: convierte el JSON en estructuras, comprobando tipos y campos.
///   2. Validación: comprueba reglas que cruzan varias entidades.
///
/// `where` es la ruta legible del dato ("medicamentos[amoxicilina]") que se
/// antepone a cada problema para que el equipo clínico lo pueda ubicar.
class Loader {
 public:
  KnowledgeBase load(const json& root) {
    KnowledgeBase kb;
    if (!root.is_object()) {
      issue("raíz", "se esperaba un objeto JSON");
      return kb;
    }

    const std::string where = "raíz";
    if (const auto schema = optionalInt(root, "esquema_version", where)) {
      kb.schema_version = *schema;
      if (*schema != kSupportedSchemaVersion) {
        issue(where, "esquema_version " + std::to_string(*schema) +
                         " no es compatible (este programa lee la versión " +
                         std::to_string(kSupportedSchemaVersion) + ")");
      }
    } else {
      issue(where, "falta el campo «esquema_version»");
    }
    kb.version = requiredString(root, "version", where);
    kb.date = requiredString(root, "fecha", where);

    for (const json& item : arrayOf(root, "fuentes", where)) {
      kb.sources.push_back(readSource(item));
    }
    for (const json& item : arrayOf(root, "medicamentos", where)) {
      kb.drugs.push_back(readDrug(item));
    }
    for (const json& item : arrayOf(root, "enfermedades", where)) {
      kb.diseases.push_back(readDisease(item));
    }

    validate(kb);
    return kb;
  }

  [[nodiscard]] std::vector<std::string>& issues() noexcept { return issues_; }

 private:
  // --- Utilidades de lectura -------------------------------------------------

  void issue(const std::string& where, const std::string& what) {
    issues_.push_back(where + ": " + what);
  }

  /// Devuelve el campo si existe y no es null.
  static const json* field(const json& object, const char* key) {
    if (!object.is_object()) {
      return nullptr;
    }
    const auto it = object.find(key);
    return (it == object.end() || it->is_null()) ? nullptr : &*it;
  }

  std::string optionalString(const json& object, const char* key, const std::string& where) {
    const json* value = field(object, key);
    if (value == nullptr) {
      return "";
    }
    if (!value->is_string()) {
      issue(where, std::string("el campo «") + key + "» debe ser texto");
      return "";
    }
    return value->get<std::string>();
  }

  std::string requiredString(const json& object, const char* key, const std::string& where) {
    std::string value = optionalString(object, key, where);
    if (value.empty() && field(object, key) == nullptr) {
      issue(where, std::string("falta el campo «") + key + "»");
    } else if (value.empty()) {
      issue(where, std::string("el campo «") + key + "» está vacío");
    }
    return value;
  }

  std::optional<double> optionalNumber(const json& object, const char* key,
                                       const std::string& where) {
    const json* value = field(object, key);
    if (value == nullptr) {
      return std::nullopt;
    }
    if (!value->is_number()) {
      issue(where, std::string("el campo «") + key + "» debe ser un número");
      return std::nullopt;
    }
    return value->get<double>();
  }

  double requiredNumber(const json& object, const char* key, const std::string& where) {
    if (field(object, key) == nullptr) {
      issue(where, std::string("falta el campo «") + key + "»");
      return 0.0;
    }
    return optionalNumber(object, key, where).value_or(0.0);
  }

  std::optional<int> optionalInt(const json& object, const char* key, const std::string& where) {
    const json* value = field(object, key);
    if (value == nullptr) {
      return std::nullopt;
    }
    if (!value->is_number_integer()) {
      issue(where, std::string("el campo «") + key + "» debe ser un número entero");
      return std::nullopt;
    }
    return value->get<int>();
  }

  bool optionalBool(const json& object, const char* key, const std::string& where, bool fallback) {
    const json* value = field(object, key);
    if (value == nullptr) {
      return fallback;
    }
    if (!value->is_boolean()) {
      issue(where, std::string("el campo «") + key + "» debe ser true o false");
      return fallback;
    }
    return value->get<bool>();
  }

  /// Devuelve la lista del campo, o una lista vacía si falta o no es una lista.
  const json& arrayOf(const json& object, const char* key, const std::string& where) {
    static const json kEmpty = json::array();
    const json* value = field(object, key);
    if (value == nullptr) {
      return kEmpty;
    }
    if (!value->is_array()) {
      issue(where, std::string("el campo «") + key + "» debe ser una lista");
      return kEmpty;
    }
    return *value;
  }

  /// Convierte un texto del JSON en un valor de enumeración según `table`.
  template <typename Enum>
  Enum readEnum(const json& object, const char* key, const std::string& where,
                std::initializer_list<std::pair<const char*, Enum>> table, Enum fallback) {
    const std::string text = requiredString(object, key, where);
    if (text.empty()) {
      return fallback;
    }
    std::string allowed;
    for (const auto& [name, value] : table) {
      if (text == name) {
        return value;
      }
      allowed += allowed.empty() ? name : std::string(", ") + name;
    }
    issue(where, std::string("valor «") + text + "» no reconocido en «" + key +
                     "» (valores permitidos: " + allowed + ")");
    return fallback;
  }

  Route readRoute(const json& object, const std::string& where) {
    return readEnum<Route>(object, "via", where,
                           {{"oral", Route::Oral},
                            {"iv", Route::Intravenous},
                            {"im", Route::Intramuscular},
                            {"topica", Route::Topical}},
                           Route::Oral);
  }

  ReviewStatus readStatus(const json& object, const std::string& where) {
    return readEnum<ReviewStatus>(object, "estado", where,
                                  {{"muestra", ReviewStatus::Sample},
                                   {"borrador", ReviewStatus::Draft},
                                   {"verificado", ReviewStatus::Verified}},
                                  ReviewStatus::Sample);
  }

  // --- Lectura de entidades --------------------------------------------------

  Source readSource(const json& item) {
    Source source;
    source.id = requiredString(item, "id", "fuentes[?]");
    const std::string where = "fuentes[" + source.id + "]";
    source.title = requiredString(item, "titulo", where);
    source.organization = optionalString(item, "organizacion", where);
    source.year = optionalInt(item, "anio", where).value_or(0);
    source.url = optionalString(item, "url", where);
    return source;
  }

  Presentation readPresentation(const json& item, const std::string& drug_where) {
    Presentation presentation;
    presentation.id = requiredString(item, "id", drug_where + ".presentaciones[?]");
    const std::string where = drug_where + ".presentaciones[" + presentation.id + "]";
    presentation.description = requiredString(item, "descripcion", where);
    presentation.route = readRoute(item, where);
    presentation.kind = readEnum<PresentationKind>(
        item, "tipo", where,
        {{"liquido", PresentationKind::Liquid}, {"solido", PresentationKind::Solid}},
        PresentationKind::Liquid);

    const double amount = requiredNumber(item, "cantidad", where);
    if (!(amount > 0.0)) {
      issue(where, "«cantidad» debe ser mayor que 0");
    }

    if (presentation.kind == PresentationKind::Liquid) {
      // Líquidos: cantidad por volumen (p. ej. 250 mg en 5 mL) → por mL.
      const double volume = requiredNumber(item, "volumen_ml", where);
      if (!(volume > 0.0)) {
        issue(where, "«volumen_ml» debe ser mayor que 0 en presentaciones líquidas");
      } else {
        presentation.amount_per_unit = amount / volume;
      }
      presentation.dispensing_unit = "mL";
      presentation.rounding_step = 0.5;
    } else {
      // Sólidos: cantidad por unidad (tableta, cápsula, vial).
      presentation.amount_per_unit = amount;
      presentation.dispensing_unit = requiredString(item, "unidad", where);
      presentation.rounding_step = optionalBool(item, "divisible", where, false) ? 0.5 : 1.0;
    }

    if (const auto step = optionalNumber(item, "paso_redondeo", where)) {
      if (!(*step > 0.0)) {
        issue(where, "«paso_redondeo» debe ser mayor que 0");
      } else {
        presentation.rounding_step = *step;
      }
    }
    return presentation;
  }

  RenalAdjustment readRenal(const json& item, const std::string& drug_where) {
    const std::string where = drug_where + ".ajuste_renal";
    RenalAdjustment renal;
    renal.kind = readEnum<RenalAdjustmentKind>(item, "tipo", where,
                                               {{"no_requiere", RenalAdjustmentKind::NotRequired},
                                                {"por_bandas", RenalAdjustmentKind::Banded},
                                                {"sin_dato", RenalAdjustmentKind::NoData}},
                                               RenalAdjustmentKind::NoData);
    renal.note = optionalString(item, "nota", where);
    renal.source_id = optionalString(item, "fuente_id", where);
    renal.status = readStatus(item, where);

    for (const json& band_item : arrayOf(item, "bandas", where)) {
      RenalBand band;
      band.gfr_min = requiredNumber(band_item, "tfg_min", where + ".bandas");
      band.gfr_max = requiredNumber(band_item, "tfg_max", where + ".bandas");
      band.dose_factor = optionalNumber(band_item, "factor_dosis", where + ".bandas").value_or(1.0);
      band.min_interval_hours = optionalInt(band_item, "intervalo_minimo_horas", where + ".bandas");
      band.replacement_dose_per_kg = optionalNumber(band_item, "dosis_por_kg", where + ".bandas");
      band.max_per_dose = optionalNumber(band_item, "max_por_dosis", where + ".bandas");
      band.avoid = optionalBool(band_item, "evitar", where + ".bandas", false);
      band.note = optionalString(band_item, "nota", where + ".bandas");
      renal.bands.push_back(std::move(band));
    }
    return renal;
  }

  Drug readDrug(const json& item) {
    Drug drug;
    drug.id = requiredString(item, "id", "medicamentos[?]");
    const std::string where = "medicamentos[" + drug.id + "]";
    drug.name = requiredString(item, "nombre", where);
    drug.unit = readEnum<DrugUnit>(
        item, "unidad", where,
        {{"mg", DrugUnit::Milligram}, {"UI", DrugUnit::InternationalUnit}}, DrugUnit::Milligram);

    for (const json& presentation : arrayOf(item, "presentaciones", where)) {
      drug.presentations.push_back(readPresentation(presentation, where));
    }
    if (const json* renal = field(item, "ajuste_renal")) {
      drug.renal = readRenal(*renal, where);
    } else {
      issue(where, "falta el campo «ajuste_renal» (use tipo «sin_dato» si aún no se ha compilado)");
    }
    return drug;
  }

  Regimen readRegimen(const json& item, const std::string& option_where) {
    Regimen regimen;
    regimen.drug_id = requiredString(item, "medicamento_id", option_where);
    const std::string where = option_where + "." + regimen.drug_id;

    regimen.route = readRoute(item, where);
    regimen.basis = readEnum<DoseBasis>(item, "base", where,
                                        {{"mg_kg_dia", DoseBasis::PerKgPerDay},
                                         {"mg_kg_dosis", DoseBasis::PerKgPerDose},
                                         {"dosis_fija", DoseBasis::Fixed},
                                         {"sin_calculo", DoseBasis::NoCalculation}},
                                        DoseBasis::PerKgPerDay);

    if (regimen.basis != DoseBasis::NoCalculation) {
      if (const json* dose = field(item, "dosis")) {
        regimen.dose_min = requiredNumber(*dose, "min", where + ".dosis");
        regimen.dose_usual = requiredNumber(*dose, "usual", where + ".dosis");
        regimen.dose_max = requiredNumber(*dose, "max", where + ".dosis");
      } else {
        issue(where, "falta el campo «dosis»");
      }
    }

    regimen.interval_hours = optionalInt(item, "intervalo_horas", where);
    regimen.max_per_dose = optionalNumber(item, "max_por_dosis", where);
    regimen.max_per_day = optionalNumber(item, "max_por_dia", where);

    if (const json* duration = field(item, "duracion_dias")) {
      regimen.duration_min_days = optionalInt(*duration, "min", where + ".duracion_dias");
      regimen.duration_max_days = optionalInt(*duration, "max", where + ".duracion_dias");
    }
    if (const json* age = field(item, "edad_meses")) {
      regimen.age_min_months = optionalInt(*age, "min", where + ".edad_meses");
      regimen.age_max_months = optionalInt(*age, "max", where + ".edad_meses");
    }
    if (const json* weight = field(item, "peso_kg")) {
      regimen.weight_min_kg = optionalNumber(*weight, "min", where + ".peso_kg");
      regimen.weight_max_kg = optionalNumber(*weight, "max", where + ".peso_kg");
    }

    regimen.note = optionalString(item, "nota", where);
    regimen.source_id = requiredString(item, "fuente_id", where);
    regimen.page = optionalString(item, "pagina", where);
    regimen.status = readStatus(item, where);
    for (const json& verifier : arrayOf(item, "verificado_por", where)) {
      if (verifier.is_string() && !verifier.get<std::string>().empty()) {
        regimen.verified_by.push_back(verifier.get<std::string>());
      }
    }
    return regimen;
  }

  Disease readDisease(const json& item) {
    Disease disease;
    disease.id = requiredString(item, "id", "enfermedades[?]");
    const std::string where = "enfermedades[" + disease.id + "]";
    disease.name = requiredString(item, "nombre", where);
    disease.group = optionalString(item, "grupo", where);
    disease.antibiotic_indicated = optionalBool(item, "requiere_antibiotico", where, true);
    disease.message = optionalString(item, "mensaje", where);
    disease.source_id = optionalString(item, "fuente_id", where);

    for (const json& line_item : arrayOf(item, "lineas", where)) {
      TreatmentLine line;
      line.kind = readEnum<TreatmentLineKind>(line_item, "linea", where,
                                              {{"primera", TreatmentLineKind::First},
                                               {"segunda", TreatmentLineKind::Second},
                                               {"alergia", TreatmentLineKind::Allergy}},
                                              TreatmentLineKind::First);
      const std::string line_where = where + "." + optionalString(line_item, "linea", where);

      for (const json& option_item : arrayOf(line_item, "opciones", line_where)) {
        TherapyOption option;
        option.number = optionalInt(option_item, "numero", line_where).value_or(0);
        const std::string option_where = line_where + ".opción " + std::to_string(option.number);
        option.description = requiredString(option_item, "descripcion", option_where);
        for (const json& component : arrayOf(option_item, "componentes", option_where)) {
          option.components.push_back(readRegimen(component, option_where));
        }
        line.options.push_back(std::move(option));
      }
      disease.lines.push_back(std::move(line));
    }
    return disease;
  }

  // --- Validación cruzada ----------------------------------------------------

  /// Comprueba que los identificadores no se repitan y devuelve el conjunto.
  template <typename T>
  std::set<std::string> uniqueIds(const std::vector<T>& items, const std::string& collection) {
    std::set<std::string> ids;
    for (const T& item : items) {
      if (!item.id.empty() && !ids.insert(item.id).second) {
        issue(collection, "identificador repetido «" + item.id + "»");
      }
    }
    return ids;
  }

  void validate(const KnowledgeBase& kb) {
    const std::set<std::string> source_ids = uniqueIds(kb.sources, "fuentes");
    uniqueIds(kb.drugs, "medicamentos");
    uniqueIds(kb.diseases, "enfermedades");

    // Los identificadores de presentación son únicos en toda la base porque la
    // matriz de validación alfa los referencia sin nombrar el medicamento.
    std::vector<Presentation> all_presentations;
    for (const Drug& drug : kb.drugs) {
      all_presentations.insert(all_presentations.end(), drug.presentations.begin(),
                               drug.presentations.end());
      validateRenal(drug, source_ids);
    }
    uniqueIds(all_presentations, "presentaciones");

    for (const Disease& disease : kb.diseases) {
      validateDisease(kb, disease, source_ids);
    }
  }

  void validateRenal(const Drug& drug, const std::set<std::string>& source_ids) {
    const std::string where = "medicamentos[" + drug.id + "].ajuste_renal";
    const RenalAdjustment& renal = drug.renal;

    if (!renal.source_id.empty() && source_ids.count(renal.source_id) == 0) {
      issue(where, "la fuente «" + renal.source_id + "» no existe");
    }
    if (renal.status == ReviewStatus::Verified && renal.source_id.empty()) {
      issue(where, "un ajuste renal verificado debe citar su fuente");
    }
    if (renal.kind != RenalAdjustmentKind::Banded) {
      if (!renal.bands.empty()) {
        issue(where, "solo el tipo «por_bandas» puede tener bandas");
      }
      return;
    }
    if (renal.bands.empty()) {
      issue(where, "el tipo «por_bandas» necesita al menos una banda");
      return;
    }

    for (const RenalBand& band : renal.bands) {
      if (band.gfr_min < 0.0 || !(band.gfr_min < band.gfr_max)) {
        issue(where, "banda inválida: se requiere 0 <= tfg_min < tfg_max");
      }
      if (!(band.dose_factor > 0.0) || band.dose_factor > 1.0) {
        issue(where, "«factor_dosis» debe estar entre 0 (excluido) y 1");
      }
      if (band.min_interval_hours && *band.min_interval_hours <= 0) {
        issue(where, "«intervalo_minimo_horas» debe ser mayor que 0");
      }
      if (band.replacement_dose_per_kg && !(*band.replacement_dose_per_kg > 0.0)) {
        issue(where, "«dosis_por_kg» de la banda debe ser mayor que 0");
      }
      if (band.max_per_dose && !(*band.max_per_dose > 0.0)) {
        issue(where, "«max_por_dosis» de la banda debe ser mayor que 0");
      }
    }

    // Solapamiento: con las bandas ordenadas, cada una debe empezar donde
    // termina la anterior o después.
    std::vector<RenalBand> sorted = renal.bands;
    std::sort(sorted.begin(), sorted.end(),
              [](const RenalBand& a, const RenalBand& b) { return a.gfr_min < b.gfr_min; });
    for (std::size_t i = 1; i < sorted.size(); ++i) {
      if (sorted[i].gfr_min < sorted[i - 1].gfr_max) {
        issue(where, "las bandas de TFG se solapan");
        break;
      }
    }
  }

  void validateDisease(const KnowledgeBase& kb, const Disease& disease,
                       const std::set<std::string>& source_ids) {
    const std::string where = "enfermedades[" + disease.id + "]";

    if (!disease.source_id.empty() && source_ids.count(disease.source_id) == 0) {
      issue(where, "la fuente «" + disease.source_id + "» no existe");
    }
    if (!disease.antibiotic_indicated) {
      if (disease.message.empty()) {
        issue(where, "una enfermedad sin antibiótico necesita el campo «mensaje»");
      }
      if (!disease.lines.empty()) {
        issue(where, "una enfermedad sin antibiótico no puede tener líneas de manejo");
      }
      return;
    }
    if (disease.lines.empty()) {
      issue(where, "no tiene líneas de manejo");
    }

    std::set<TreatmentLineKind> seen_lines;
    for (const TreatmentLine& line : disease.lines) {
      const std::string line_where = where + "." + std::string(label(line.kind));
      if (!seen_lines.insert(line.kind).second) {
        issue(line_where, "la línea está repetida");
      }
      if (line.options.empty()) {
        issue(line_where, "no tiene opciones");
      }

      std::set<int> seen_numbers;
      for (const TherapyOption& option : line.options) {
        const std::string option_where = line_where + ".opción " + std::to_string(option.number);
        if (option.number <= 0) {
          issue(option_where, "«numero» debe ser un entero mayor que 0");
        } else if (!seen_numbers.insert(option.number).second) {
          issue(option_where, "número de opción repetido dentro de la línea");
        }
        if (option.components.empty()) {
          issue(option_where, "no tiene componentes");
        }
        for (const Regimen& regimen : option.components) {
          validateRegimen(kb, regimen, option_where + "." + regimen.drug_id, source_ids);
        }
      }
    }
  }

  void validateRegimen(const KnowledgeBase& kb, const Regimen& regimen, const std::string& where,
                       const std::set<std::string>& source_ids) {
    if (!regimen.drug_id.empty() && kb.findDrug(regimen.drug_id) == nullptr) {
      issue(where, "el medicamento «" + regimen.drug_id + "» no existe");
    }
    if (!regimen.source_id.empty() && source_ids.count(regimen.source_id) == 0) {
      issue(where, "la fuente «" + regimen.source_id + "» no existe");
    }

    if (regimen.basis == DoseBasis::NoCalculation) {
      if (regimen.note.empty()) {
        issue(where, "un esquema «sin_calculo» necesita la indicación en el campo «nota»");
      }
    } else if (!(regimen.dose_min > 0.0) || regimen.dose_min > regimen.dose_usual ||
               regimen.dose_usual > regimen.dose_max) {
      issue(where, "dosis inválida: se requiere 0 < min <= usual <= max");
    }

    if (regimen.basis == DoseBasis::PerKgPerDay && !regimen.interval_hours) {
      issue(where, "la base «mg_kg_dia» necesita «intervalo_horas»");
    }
    if (regimen.interval_hours && *regimen.interval_hours <= 0) {
      issue(where, "«intervalo_horas» debe ser mayor que 0 (omítalo si es dosis única)");
    }
    if (regimen.max_per_dose && !(*regimen.max_per_dose > 0.0)) {
      issue(where, "«max_por_dosis» debe ser mayor que 0");
    }
    if (regimen.max_per_day) {
      if (!(*regimen.max_per_day > 0.0)) {
        issue(where, "«max_por_dia» debe ser mayor que 0");
      }
      if (!regimen.interval_hours) {
        issue(where, "«max_por_dia» solo aplica si hay «intervalo_horas»");
      }
    }

    validateRange(regimen.duration_min_days, regimen.duration_max_days, 1, where, "duracion_dias");
    validateRange(regimen.age_min_months, regimen.age_max_months, 0, where, "edad_meses");
    validateRange(regimen.weight_min_kg, regimen.weight_max_kg, 0.0, where, "peso_kg");
    // La duración admite min == max; edad y peso son rangos semiabiertos y no.
    if (regimen.age_min_months && regimen.age_max_months &&
        *regimen.age_min_months == *regimen.age_max_months) {
      issue(where, "«edad_meses»: min y max no pueden ser iguales (rango vacío)");
    }
    if (regimen.weight_min_kg && regimen.weight_max_kg &&
        *regimen.weight_min_kg == *regimen.weight_max_kg) {
      issue(where, "«peso_kg»: min y max no pueden ser iguales (rango vacío)");
    }

    if (regimen.status == ReviewStatus::Verified) {
      const std::set<std::string> verifiers(regimen.verified_by.begin(), regimen.verified_by.end());
      if (verifiers.size() < 2) {
        issue(where, "el estado «verificado» exige dos verificadores distintos en «verificado_por»");
      }
    }
  }

  /// Comprueba lowest <= min <= max para un rango con extremos opcionales.
  template <typename Number>
  void validateRange(const std::optional<Number>& min, const std::optional<Number>& max,
                     Number lowest, const std::string& where, const char* name) {
    if ((min && *min < lowest) || (max && *max < lowest)) {
      issue(where, std::string("«") + name + "» tiene un valor por debajo del mínimo permitido");
    }
    if (min && max && *min > *max) {
      issue(where, std::string("«") + name + "»: min no puede ser mayor que max");
    }
  }

  std::vector<std::string> issues_;
};

}  // namespace

KnowledgeBaseError::KnowledgeBaseError(std::vector<std::string> issues)
    : std::runtime_error(buildMessage(issues)), issues_(std::move(issues)) {}

KnowledgeBase loadKnowledgeBaseFromString(std::string_view json_text) {
  json root;
  try {
    root = json::parse(json_text);
  } catch (const json::parse_error& error) {
    throw KnowledgeBaseError({std::string("el archivo no es JSON válido: ") + error.what()});
  }

  Loader loader;
  KnowledgeBase kb = loader.load(root);
  if (!loader.issues().empty()) {
    throw KnowledgeBaseError(std::move(loader.issues()));
  }
  return kb;
}

KnowledgeBase loadKnowledgeBaseFromFile(const std::filesystem::path& path) {
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    throw KnowledgeBaseError({"no se pudo abrir el archivo: " + path.string()});
  }
  std::ostringstream buffer;
  buffer << file.rdbuf();
  return loadKnowledgeBaseFromString(buffer.str());
}

}  // namespace calc
