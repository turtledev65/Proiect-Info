#include "area.hpp"

namespace Statistics
{
Area::Area(const std::string &n) : m_Name(n)
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

Sex Area::getSexTotal() const
{
  Sex out;
  for (int i = 0; i < ETHNICITY_COUNT; i++) {
    out.male += m_Ethnicities[i].sex.male;
    out.female += m_Ethnicities[i].sex.female;
  }
  return out;
}

size_t Area::getTotalPopulation() const
{
  size_t out = 0;
  for (size_t i = 0; i < ETHNICITY_COUNT; i++) {
    out += m_Ethnicities[i].sex.getTotal();
  }
  return out;
}

std::string Area::getId() const
{
  if (s_AreaMap.find(m_Name) == s_AreaMap.end())
    return "";

  return s_AreaMap.at(m_Name);
}

} // namespace Statistics
