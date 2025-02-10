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

std::vector<Area> parseCSV(const std::filesystem::path &path)
{
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("File not found");
  }

  Csv::Reader reader =
      Csv::Reader(file).SetDelimiter(',').SetQuote('"').SetNewLine('\n');

  // state of the reader
  enum class State {
    IN_AREA_ROW = 0,
    IN_STATISTICS_ROW,
    IN_MALE_ROW,
    IN_FEMALE_ROW,
  };
  State state;

  std::vector<Area> areas;
  areas.reserve(43);

  for (auto &row : reader) {
    if (row.size() == 2) {
      state = State::IN_AREA_ROW;
    } else {
      state = State::IN_STATISTICS_ROW;
    }

    size_t colIdx = 0;
    for (auto &field : row) {
      switch (state) {
      case State::IN_AREA_ROW:
        if (field.size() > 0) {
          areas.push_back(field);
        }
        break;
      case State::IN_STATISTICS_ROW:
        if (field == "Masculin") {
          state = State::IN_MALE_ROW;
        } else if (field == "Feminin") {
          state = State::IN_FEMALE_ROW;
        }
        break;
      case State::IN_MALE_ROW:
        if (field.size() > 0 && field != "*" && field != "-") {
          size_t count                                    = stoi(field);
          areas.back().m_Ethnicities[colIdx - 1].sex.male = count;
        }
        break;
      case State::IN_FEMALE_ROW:
        if (field.size() > 0 && field != "*" && field != "-") {
          size_t count                                      = stoi(field);
          areas.back().m_Ethnicities[colIdx - 1].sex.female = count;
        }
        break;
      default:
        break;
      }
      colIdx++;
    }
  }

  return areas;
}

} // namespace Statistics
