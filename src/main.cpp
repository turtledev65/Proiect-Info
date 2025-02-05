#include "Csv/parser.hpp"
#include "Html/generator.hpp"
#include <fstream>

#include "Statistics/area.hpp"
#include "Core/utils.hpp"

using namespace Statistics;

// Functions
std::vector<Area> readCsvFile(const std::string &path);
std::string       convertAreasToJson(const std::vector<Area> &areas);

// Main
int main()
{
  // Read data from csv file
  std::vector<Area> areas = readCsvFile("res/data/Recensamant-2021.csv");

  // Generate the html document
  Html::Generator generator("res/statistics-website-template/index.html");
  generator.define("AREAS", Html::ConvertType::JSON,
                   [areas]() { return convertAreasToJson(areas); });
  generator.parseTemplateToFile("./index.html");
  Utils::systemOpenHTMLDoc("./index.html");
}

// Function Implementations
std::string convertAreasToJson(const std::vector<Area> &areas)
{
  std::stringstream out;

  out << "`{";
  for (size_t i = 0; i < areas.size(); i++) {
    const Area &area = areas[i];
    out << area.toJSON();
    if (i < areas.size() - 1) {
      out << ',';
    }
  }
  out << "}`";

  return out.str();
}

std::vector<Area> readCsvFile(const std::string &path)
{
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("File not found");
  }

  Csv::Parser parser =
      Csv::Parser(file).delimiter(',').quote('"').newLine('\n');

  // state of the parser
  enum class State {
    IN_AREA_ROW = 0,
    IN_STATISTICS_ROW,
    IN_MALE_ROW,
    IN_FEMALE_ROW,
  };
  State state;

  std::vector<Area> areas;
  areas.reserve(50);

  for (auto &row : parser) {
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
