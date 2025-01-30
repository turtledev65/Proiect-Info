#include <fstream>
#include "Csv/parser.hpp"

// Data Types

// Sex
struct Sex {
  Sex(size_t m = 0, size_t f = 0) : male(m), female(f) {};

  size_t male   = 0;
  size_t female = 0;

  size_t getTotal()
  {
    return male + female;
  };
};

// Ethnicity
#define ETHNICITY_COUNT 23
enum class EthnicityType {
  ROMANIAN = 0,
  HUNGARIAN,
  ROMA,
  UKRAINIAN,
  GERMAN,
  TURKISH,
  RUSSIAN_LIPOVAN,
  TATAR,
  SERB,
  SLOVAK,
  BULGARIAN,
  CROATIAN,
  GREEK,
  ITALIAN,
  JEW,
  CZECH,
  POLE,
  RUTHENIAN,
  ARMENIAN,
  ALBANIAN,
  MACEDONIAN,
  OTHER,
  INFORMATION_UNAVAILABLE,
};
struct Ethnicity {
  Ethnicity() : sex(0, 0), type(EthnicityType::ROMANIAN)
  {
  }

  Sex           sex;
  EthnicityType type;
};

// Area
struct Area {
  Area(const std::string &n) : name(n)
  {
    for (int i = 0; i < ETHNICITY_COUNT; i++) {
      ethnicities[i].type = static_cast<EthnicityType>(i);
    }
  }

  std::string name = "";
  Ethnicity   ethnicities[ETHNICITY_COUNT];
};

int main()
{
  std::ifstream file("res/Recensamant-2021.csv");
  Csv::Parser   parser =
      Csv::Parser(file).delimiter(',').quote('"').newLine('\n');

  std::vector<Area> areas;
  for (auto &row : parser) {
    for (auto &field : row) {
      bool isArea = row.size() == 2 && field.size() > 0;
      if (isArea) {
      }
    }
  }
  std::cout << areas.size() << std::endl;
  for (auto &area : areas) {
    std::cout << area.name << std::endl;
  }
}
