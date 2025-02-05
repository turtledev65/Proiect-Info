#include "area.hpp"

namespace Statistics
{
Area::Area(const std::string &n) : name(n)
{
  for (int i = 0; i < ETHNICITY_COUNT; i++) {
    ethnicities[i].type = static_cast<EthnicityType>(i);
  }
}

Sex Area::getSexTotal() const
{
  Sex out;
  for (int i = 0; i < ETHNICITY_COUNT; i++) {
    out.male += ethnicities[i].sex.male;
    out.female += ethnicities[i].sex.female;
  }
  return out;
}

size_t Area::getTotalPopulation() const
{
  size_t out = 0;
  for (size_t i; i < ETHNICITY_COUNT; i++) {
    out += ethnicities[i].sex.getTotal();
  }
  return out;
}

std::string Area::getId() const
{
  if (s_AreaMap.find(name) == s_AreaMap.end())
    return "";

  return s_AreaMap.at(name);
}

} // namespace Statistics
