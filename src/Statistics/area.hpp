#pragma once

#include <string>
#include <unordered_map>

#include "Statistics/sex.hpp"
#include "Statistics/ethnicity.hpp"

namespace Statistics
{
class Area
{
public:
  Area(const std::string &n);

  std::string name = "";
  Ethnicity   ethnicities[ETHNICITY_COUNT];

  Sex         getSexTotal() const;
  size_t      getTotalPopulation() const;
  std::string getId() const;

private:
  static const inline std::unordered_map<std::string, std::string> s_AreaMap = {
      {"ROMANIA", "RO"},
      {"ALBA", "AB"},
      {"BISTRITA-NASAUD", "BN"},
      {"BRASOV", "BV"},
      {"CLUJ", "CJ"},
      {"COVASNA", "CV"},
      {"HUNEDOARA", "HD"},
      {"HARGHITA", "HR"},
      {"MURES", "MS"},
      {"SIBIU", "SB"},
      {"SALAJ", "SJ"},
      {"ARGES", "AG"},
      {"BRAILA", "BR"},
      {"MUNICIPIUL BUCURESTI", "B"},
      {"BUZAU", "BZ"},
      {"CALARASI", "CL"},
      {"DAMBOVITA", "DB"},
      {"GIURGIU", "GR"},
      {"ILFOV", "IF"},
      {"IALOMITA", "IL"},
      {"PRAHOVA", "PH"},
      {"TELEORMAN", "TR"},
      {"ARAD", "AR"},
      {"BIHOR", "BH"},
      {"BACAU", "BC"},
      {"BOTOSANI", "BT"},
      {"GALATI", "GL"},
      {"IASI", "IS"},
      {"NEAMT", "NT"},
      {"VRANCEA", "VN"},
      {"VASLUI", "VS"},
      {"CARAS-SEVERIN", "CS"},
      {"TIMIS", "TM"},
      {"CONSTANTA", "CT"},
      {"TULCEA", "TL"},
      {"DOLJ", "DJ"},
      {"GORJ", "GJ"},
      {"MEHEDINTI", "MH"},
      {"OLT", "OT"},
      {"VALCEA", "VL"},
      {"MARAMURES", "MM"},
      {"SATU MARE", "SM"},
      {"SUCEAVA", "SV"}};
};

} // namespace Statistics
