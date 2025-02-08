#include "Csv/reader.hpp"
#include "Html/generator.hpp"
#include "Statistics/area.hpp"
#include "Core/utils.hpp"

#include <fstream>

using namespace std;
using namespace Statistics;

// Functions
vector<Area> readCsvFile(const string &path);
string       convertAreasToJson(const vector<Area> &areas);

// Main
int main()
{
  // Read data from csv file
  vector<Area> areas = readCsvFile("res/data/Recensamant-2021.csv");

  // Generate the html document
  Html::Generator generator("res/statistics-website-template/index.html");
  generator.define("AREAS", Html::ConvertType::JSON,
                   [areas]() { return convertAreasToJson(areas); });
  generator.parseTemplateToFile("./index.html");
}

// Function Implementations
string convertAreasToJson(const vector<Area> &areas)
{
  stringstream out;

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

vector<Area> readCsvFile(const string &path)
{
  ifstream file(path);
  if (!file.is_open()) {
    throw runtime_error("File not found");
  }

  Csv::Reader reader =
      Csv::Reader(file).delimiter(',').quote('"').newLine('\n');

  // state of the reader
  enum class State {
    IN_AREA_ROW = 0,
    IN_STATISTICS_ROW,
    IN_MALE_ROW,
    IN_FEMALE_ROW,
  };
  State state;

  vector<Area> areas;
  areas.reserve(50);

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
