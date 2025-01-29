#include <fstream>
#include "Csv/parser.hpp"

struct Area {
  Area(const std::string &str) : name(str)
  {
  }

  std::string name = "";
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
        areas.emplace_back(std::move(field));
      }
    }
  }
  std::cout << areas.size() << std::endl;
  for (auto &area : areas) {
    std::cout << area.name << std::endl;
  }
}
