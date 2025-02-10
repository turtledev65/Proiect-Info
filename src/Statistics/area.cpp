#include "area.hpp"

namespace Statistics
{
Area::Area(const std::string &name) : m_Name(std::move(name))
{
  for (int i = 0; i < ETHNICITY_COUNT; i++) {
    m_Ethnicities[i].type = static_cast<EthnicityType>(i);
  }
}

std::string Area::ToJSON() const
{
  std::stringstream stream;

  stream << '"' << GetId() << '"' << ':';
  stream << '{';

  stream << "\"name\": " << '"' << m_Name << '"' << ','
         << "\"totalPopulation\": " << GetTotalPopulation() << ','
         << "\"historicalRegion\": " << '"' << GetHistoricalRegion() << '"'
         << ',' << "\"ethnicities\": ";

  stream << '[';
  for (size_t i = 0; i < ETHNICITY_COUNT; i++) {
    stream << m_Ethnicities[i].ToJSON();
    if (i < ETHNICITY_COUNT - 1) {
      stream << ',';
    }
  }
  stream << ']';

  stream << '}';
  return stream.str();
}

Sex Area::GetSexTotal() const { return m_Ethnicities[0].sex; }

size_t Area::GetTotalPopulation() const { return GetSexTotal().GetTotal(); }

std::string Area::GetId() const
{
  if (s_AreaMap.find(m_Name) == s_AreaMap.end()) {
    return "";
  }

  return s_AreaMap.at(m_Name).id;
}

std::string Area::GetHistoricalRegion() const
{
  if (s_AreaMap.find(m_Name) == s_AreaMap.end()) {
    return "";
  }

  return s_AreaMap.at(m_Name).historicalRegion;
}

} // namespace Statistics
