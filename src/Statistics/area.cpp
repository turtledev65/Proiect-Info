#include "area.hpp"

namespace Statistics
{
Area::Area(const std::string &name) : m_Name(name)
{
  for (int i = 0; i < ETHNICITY_COUNT; i++) {
    m_Ethnicities[i].type = static_cast<EthnicityType>(i);
  }
}

std::string Area::toJSON() const
{
  std::stringstream stream;

  stream << '"' << getId() << '"' << ':';
  stream << '{';

  stream << "\"name\": " << '"' << m_Name << '"' << ','
         << "\"totalPopulation\": " << getTotalPopulation() << ','
         << "\"historicalRegion\": " << '"' << getHistoricalRegion() << '"' << ','
         << "\"ethnicities\": ";

  stream << '[';
  for (size_t i = 0; i < ETHNICITY_COUNT; i++) {
    stream << m_Ethnicities[i].toJSON();
    if (i < ETHNICITY_COUNT - 1) {
      stream << ',';
    }
  }
  stream << ']';

  stream << '}';
  return stream.str();
}

Sex Area::getSexTotal() const { return m_Ethnicities[0].sex; }

size_t Area::getTotalPopulation() const { return getSexTotal().getTotal(); }

std::string Area::getId() const
{
  if (s_AreaIDMap.find(m_Name) == s_AreaIDMap.end()) {
    return "";
  }

  return s_AreaIDMap.at(m_Name);
}

std::string Area::getHistoricalRegion() const
{
  if (s_AreaRegionMap.find(m_Name) == s_AreaRegionMap.end()) {
    return "";
  }

  return s_AreaRegionMap.at(m_Name);
}

} // namespace Statistics
