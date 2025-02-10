#pragma once

#include <string>
#include <sstream>

#include "Statistics/sex.hpp"

namespace Statistics
{
enum class EthnicityType {
  TOTAL = 0,
  ROMANIAN,
  HUNGARIAN,
  ROMA,
  UKRAINIAN,
  GERMAN,
  TURKISH,
  RUSSIAN_LIPOVAN,
  TATAR,
  SERB,
  SLOVAK,
  BULGARIAN,
  CROATIAN,
  GREEK,
  ITALIAN,
  JEW,
  CZECH,
  POLE,
  RUTHENIAN,
  ARMENIAN,
  ALBANIAN,
  MACEDONIAN,
  OTHER,
  INFORMATION_UNAVAILABLE,
  COUNT, // must be last
};
constexpr size_t ETHNICITY_COUNT =
    static_cast<size_t>(Statistics::EthnicityType::COUNT);

struct Ethnicity {
  Ethnicity() : sex(0, 0), type(EthnicityType::ROMANIAN) {}

  Sex           sex;
  EthnicityType type;

  std::string ToJSON() const;
  std::string GetName() const;
};
} // namespace Statistics
