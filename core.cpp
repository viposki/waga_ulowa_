#include "core.hpp" 

namespace core
{
  String fullPrettyValueText(String prefix, float value, String unit)
  {
    return prefix + " " + prettyValueText(value, unit);
  }

  String prettyValueText(float value, String unit)
  {
    return String(value) + " " + unit;
  }
} // namespace core