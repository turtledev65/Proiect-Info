#pragma once

#include "ethnicity.hpp"

namespace Statistics
{
std::string Ethnicity::toJSON() const
{
  std::stringstream stream;
  stream << '{';
  stream << "\"type\": " << static_cast<size_t>(type) << ','
         << "\"name\": " << '"' << getName() << '"' << ','
         << "\"total\": " << sex.getTotal();
  stream << '}';
  return stream.str();
}

std::string Ethnicity::getName() const
{
  switch (type) {
  case EthnicityType::TOTAL:
    return "Total";
  case EthnicityType::ROMANIAN:
    return "Romani";
  case EthnicityType::HUNGARIAN:
    return "Maghiari";
  case EthnicityType::ROMA:
    return "Romi";
  case EthnicityType::UKRAINIAN:
    return "Ucraineni";
  case EthnicityType::GERMAN:
    return "Germani";
  case EthnicityType::TURKISH:
    return "Turci";
  case EthnicityType::RUSSIAN_LIPOVAN:
    return "Rusi-Lipoveni";
  case EthnicityType::TATAR:
    return "Tatari";
  case EthnicityType::SERB:
    return "Sarbi";
  case EthnicityType::SLOVAK:
    return "Slovaci";
  case EthnicityType::BULGARIAN:
    return "Bulgari";
  case EthnicityType::CROATIAN:
    return "Croati";
  case EthnicityType::GREEK:
    return "Greci";
  case EthnicityType::ITALIAN:
    return "Italian";
  case EthnicityType::JEW:
    return "Evrei";
  case EthnicityType::CZECH:
    return "Cehi";
  case EthnicityType::POLE:
    return "Polonezi";
  case EthnicityType::RUTHENIAN:
    return "Ruteni";
  case EthnicityType::ARMENIAN:
    return "Armeni";
  case EthnicityType::ALBANIAN:
    return "Albanezi";
  case EthnicityType::MACEDONIAN:
    return "Macedonieni";
  case EthnicityType::OTHER:
    return "Alta etnie";
  case EthnicityType::INFORMATION_UNAVAILABLE:
    return "Informatie nedisponibila";
  default:
    return "";
  }
}
} // namespace Statistics
