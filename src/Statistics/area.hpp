#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "Statistics/sex.hpp"
#include "Statistics/ethnicity.hpp"

namespace Statistics
{
class Area
{
public:
  Area(const std::string &n);

  Ethnicity m_Ethnicities[ETHNICITY_COUNT];

  std::string toJSON() const;
  Sex         getSexTotal() const;
  size_t      getTotalPopulation() const;

  std::string getId() const;
  std::string getHistoricalRegion() const;

private:
  std::string    m_Name = "";

  static const inline std::unordered_map<std::string, std::string> s_AreaIDMap =
      {{"ROMANIA", "RO"},
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
  static const inline std::unordered_map<std::string, std::string>
      s_AreaRegionMap = {
          {"ALBA", "Transilvania"},     {"BISTRITA-NASAUD", "Transilvania"},
          {"BRASOV", "Transilvania"},   {"CLUJ", "Transilvania"},
          {"COVASNA", "Transilvania"},  {"HUNEDOARA", "Transilvania"},
          {"HARGHITA", "Transilvania"}, {"MURES", "Transilvania"},
          {"SIBIU", "Transilvania"},    {"SALAJ", "Transilvania"},

          {"ARGES", "Muntenia"},        {"BRAILA", "Muntenia"},
          {"BUCURESTI", "Muntenia"},    {"BUZAU", "Muntenia"},
          {"CALARASI", "Muntenia"},     {"DAMBOVITA", "Muntenia"},
          {"GIURGIU", "Muntenia"},      {"ILFOV", "Muntenia"},
          {"IALOMITA", "Muntenia"},     {"PRAHOVA", "Muntenia"},
          {"TELEORMAN", "Muntenia"},

          {"ARAD", "Crisana"},          {"BIHOR", "Crisana"},

          {"BACAU", "Moldova"},         {"BOTOSANI", "Moldova"},
          {"GALATI", "Moldova"},        {"IASI", "Moldova"},
          {"NEAMT", "Moldova"},         {"VRANCEA", "Moldova"},
          {"VASLUI", "Moldova"},

          {"CARAS-SEVERIN", "Banat"},   {"TIMIS", "Banat"},

          {"CONSTANTA", "Constanta"},   {"TULCEA", "Constanta"},

          {"DOLJ", "Oltenia"},          {"GORJ", "Oltenia"},
          {"MEHEDINTI", "Oltenia"},     {"OLT", "Oltenia"},
          {"VALCEA", "Oltenia"},

          {"MARAMURES", "Maramures"},   {"SATU MARE", "Maramures"},

          {"SUCEAVA", "Bucovina"}};
};

} // namespace Statistics
