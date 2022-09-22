#pragma once

#include <Arduino.h>

namespace core
{
  String fullPrettyValueText(String prefix, float value, String unit);
  String prettyValueText(float value, String unit);
} // core