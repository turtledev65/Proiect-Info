#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "Statistics/sex.hpp"
#include "Statistics/ethnicity.hpp"

namespace Statistics
{
struct AreaProperties {
  explicit AreaProperties(const std::string &id, const std::string &region)
      : id(std::move(id)), historicalRegion(std::move(region))
  {
  }

  const std::string id;
  const std::string historicalRegion;
};

class Area
{
public:
  Area(const std::string &n);

  Ethnicity m_Ethnicities[ETHNICITY_COUNT];

  std::string ToJSON() const;
  Sex         GetSexTotal() const;
  size_t      GetTotalPopulation() const;

  std::string        GetId() const;
  std::string        GetHistoricalRegion() const;
  const std::string &GetName() const { return m_Name; }

private:
  std::string m_Name = "";

  static const inline std::unordered_map<std::string, AreaProperties>
      s_AreaMap = {{"ROMANIA", AreaProperties("RO", "")},
                   {"ALBA", AreaProperties("AB", "Transilvania")},
                   {"BISTRITA-NASAUD", AreaProperties("BN", "Transilvania")},
                   {"BRASOV", AreaProperties("BV", "Transilvania")},
                   {"CLUJ", AreaProperties("CJ", "Transilvania")},
                   {"COVASNA", AreaProperties("CV", "Transilvania")},
                   {"HUNEDOARA", AreaProperties("HD", "Transilvania")},
                   {"HARGHITA", AreaProperties("HR", "Transilvania")},
                   {"MURES", AreaProperties("MS", "Transilvania")},
                   {"SIBIU", AreaProperties("SB", "Transilvania")},
                   {"SALAJ", AreaProperties("SJ", "Transilvania")},
                   {"ARGES", AreaProperties("AG", "Muntenia")},
                   {"BRAILA", AreaProperties("BR", "Muntenia")},
                   {"MUNICIPIUL BUCURESTI", AreaProperties("B", "Muntenia")},
                   {"BUZAU", AreaProperties("BZ", "Muntenia")},
                   {"CALARASI", AreaProperties("CL", "Muntenia")},
                   {"DAMBOVITA", AreaProperties("DB", "Muntenia")},
                   {"GIURGIU", AreaProperties("GR", "Muntenia")},
                   {"ILFOV", AreaProperties("IF", "Muntenia")},
                   {"IALOMITA", AreaProperties("IL", "Muntenia")},
                   {"PRAHOVA", AreaProperties("PH", "Muntenia")},
                   {"TELEORMAN", AreaProperties("TR", "Muntenia")},
                   {"ARAD", AreaProperties("AR", "Crisana")},
                   {"BIHOR", AreaProperties("BH", "Crisana")},
                   {"BACAU", AreaProperties("BC", "Moldova")},
                   {"BOTOSANI", AreaProperties("BT", "Moldova")},
                   {"GALATI", AreaProperties("GL", "Moldova")},
                   {"IASI", AreaProperties("IS", "Moldova")},
                   {"NEAMT", AreaProperties("NT", "Moldova")},
                   {"VRANCEA", AreaProperties("VN", "Moldova")},
                   {"VASLUI", AreaProperties("VS", "Moldova")},
                   {"CARAS-SEVERIN", AreaProperties("CS", "Banat")},
                   {"TIMIS", AreaProperties("TM", "Banat")},
                   {"CONSTANTA", AreaProperties("CT", "Constanta")},
                   {"TULCEA", AreaProperties("TL", "Constanta")},
                   {"DOLJ", AreaProperties("DJ", "Oltenia")},
                   {"GORJ", AreaProperties("GJ", "Oltenia")},
                   {"MEHEDINTI", AreaProperties("MH", "Oltenia")},
                   {"OLT", AreaProperties("OT", "Oltenia")},
                   {"VALCEA", AreaProperties("VL", "Oltenia")},
                   {"MARAMURES", AreaProperties("MM", "Maramures")},
                   {"SATU MARE", AreaProperties("SM", "Maramures")},
                   {"SUCEAVA", AreaProperties("SV", "Bucovina")}};
};

} // namespace Statistics
