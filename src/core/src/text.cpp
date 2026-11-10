#include "calc/text.hpp"

#include <iomanip>
#include <locale>
#include <sstream>

namespace calc {

std::string formatNumber(double value, int max_decimals) {
  // Se fija la configuración regional "clásica" para que el resultado no
  // dependa del sistema operativo ni de lo que haga la interfaz gráfica.
  std::ostringstream stream;
  stream.imbue(std::locale::classic());
  stream << std::fixed << std::setprecision(max_decimals) << value;
  std::string text = stream.str();

  // Quitar ceros de relleno: "450.00" → "450", "4.50" → "4.5".
  if (text.find('.') != std::string::npos) {
    while (text.back() == '0') {
      text.pop_back();
    }
    if (text.back() == '.') {
      text.pop_back();
    }
  }
  if (text == "-0") {
    text = "0";
  }

  const bool negative = !text.empty() && text.front() == '-';
  if (negative) {
    text.erase(0, 1);
  }
  const std::size_t dot = text.find('.');
  std::string integer_part = text.substr(0, dot);
  const std::string fraction = dot == std::string::npos ? "" : text.substr(dot + 1);

  // Agrupar miles con espacio solo desde 10 000 (cinco cifras o más).
  if (integer_part.size() >= 5) {
    for (int pos = static_cast<int>(integer_part.size()) - 3; pos > 0; pos -= 3) {
      integer_part.insert(static_cast<std::size_t>(pos), " ");
    }
  }

  std::string result = negative ? "-" : "";
  result += integer_part;
  if (!fraction.empty()) {
    result += "," + fraction;
  }
  return result;
}

std::string formatAmount(double value, DrugUnit unit) {
  // Las UI se expresan siempre en enteros; los mg admiten hasta dos decimales.
  const int decimals = unit == DrugUnit::InternationalUnit ? 0 : 2;
  return formatNumber(value, decimals) + " " + std::string(symbol(unit));
}

std::string describeInterval(const std::optional<int>& interval_hours) {
  if (!interval_hours) {
    return "dosis única";
  }
  return "cada " + std::to_string(*interval_hours) + " h";
}

std::string describeDose(const RegimenResult& result) {
  if (!result.calculated) {
    return result.note;
  }
  std::string text = formatAmount(result.dose_per_administration, result.unit) + " " +
                     describeInterval(result.interval_hours);
  if (result.daily_dose) {
    text += " (" + formatAmount(*result.daily_dose, result.unit) + "/día)";
  }
  return text;
}

std::string describeDuration(const RegimenResult& result) {
  const auto days = [](int n) { return std::to_string(n) + (n == 1 ? " día" : " días"); };
  if (result.duration_min_days && result.duration_max_days &&
      *result.duration_min_days != *result.duration_max_days) {
    return std::to_string(*result.duration_min_days) + " a " + days(*result.duration_max_days);
  }
  if (result.duration_max_days) {
    return days(*result.duration_max_days);
  }
  if (result.duration_min_days) {
    return days(*result.duration_min_days);
  }
  return "";
}

std::string describeAdministration(const Administration& administration) {
  std::string unit = administration.dispensing_unit;
  // Plural en español para unidades contables ("mL" es un símbolo: no cambia).
  if (unit != "mL" && administration.rounded_quantity > 1.0 && !unit.empty()) {
    const char last = unit.back();
    const bool vowel = last == 'a' || last == 'e' || last == 'i' || last == 'o' || last == 'u';
    unit += vowel ? "s" : "es";
  }
  return formatNumber(administration.rounded_quantity) + " " + unit;
}

std::string describeRenal(const RenalFunction& renal) {
  if (!renal.gfr) {
    return "no evaluada (se asume normal)";
  }
  std::string text = "TFG " + formatNumber(renal.gfr->value(), 1) + " mL/min/1,73 m²";
  if (renal.source == RenalSource::Estimated && !renal.method.empty()) {
    text += " (" + renal.method + ")";
  } else if (renal.source == RenalSource::EnteredDirectly) {
    text += " (digitada)";
  }
  return text;
}

}  // namespace calc
