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
#define ETHNICITY_COUNT ((int)(Statistics::EthnicityType::COUNT))

struct Ethnicity {
  Ethnicity() : sex(0, 0), type(EthnicityType::ROMANIAN) {}

  Sex           sex;
  EthnicityType type;

  std::string toJSON() const;
  std::string getName() const;
};
} // namespace Statistics
