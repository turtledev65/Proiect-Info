#include "Csv/parser.hpp"
#include "Html/generator.hpp"
#include <fstream>

// Data Types

// Sex
struct Sex {
  Sex(size_t m = 0, size_t f = 0) : male(m), female(f) {};

  size_t male   = 0;
  size_t female = 0;

  size_t getTotal() const { return male + female; };
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
  Ethnicity() : sex(0, 0), type(EthnicityType::ROMANIAN) {}

  Sex           sex;
  EthnicityType type;
};

// Area
std::unordered_map<std::string, std::string> areaMap = {
    {"ROMANIA", "RO"},
    {"ALBA", "AB"},
    {"BISTRITA-NASAUD", "BN"},
    {"BRASOV", "BV"},
    {"CLUJ", "CJ"},
    {"COVASNA", "CV"},
    {"HUNEDOARA", "HD"},
    {"HARGHITA", "HR"},
    {"MURES", "MS"},
    {"SIBIU", "SB"},
    {"SALAJ", "SJ"},
    {"ARGES", "AG"},
    {"BRAILA", "BR"},
    {"MUNICIPIUL BUCURESTI", "B"},
    {"BUZAU", "BZ"},
    {"CALARASI", "CL"},
    {"DAMBOVITA", "DB"},
    {"GIURGIU", "GR"},
    {"ILFOV", "IF"},
    {"IALOMITA", "IL"},
    {"PRAHOVA", "PH"},
    {"TELEORMAN", "TR"},
    {"ARAD", "AR"},
    {"BIHOR", "BH"},
    {"BACAU", "BC"},
    {"BOTOSANI", "BT"},
    {"GALATI", "GL"},
    {"IASI", "IS"},
    {"NEAMT", "NT"},
    {"VRANCEA", "VN"},
    {"VASLUI", "VS"},
    {"CARAS-SEVERIN", "CS"},
    {"TIMIS", "TM"},
    {"CONSTANTA", "CT"},
    {"TULCEA", "TL"},
    {"DOLJ", "DJ"},
    {"GORJ", "GJ"},
    {"MEHEDINTI", "MH"},
    {"OLT", "OT"},
    {"VALCEA", "VL"},
    {"MARAMURES", "MM"},
    {"SATU MARE", "SM"},
    {"SUCEAVA", "SV"}};

struct Area {
  Area(const std::string &n) : name(n)
  {
    for (int i = 0; i < ETHNICITY_COUNT; i++) {
      ethnicities[i].type = static_cast<EthnicityType>(i);
    }
  }

  std::string name = "";
  Ethnicity   ethnicities[ETHNICITY_COUNT];

  Sex getSexTotal()
  {
    Sex out;
    for (int i = 0; i < ETHNICITY_COUNT; i++) {
      out.male += ethnicities[i].sex.male;
      out.female += ethnicities[i].sex.female;
    }
    return out;
  }

  size_t getTotalPopulation() const
  {
    size_t out = 0;
    for (size_t i; i < ETHNICITY_COUNT; i++) {
      out += ethnicities[i].sex.getTotal();
    }
    return out;
  }

  std::string getId() const
  {
    if (areaMap.find(name) == areaMap.end())
      return "";

    return areaMap[name];
  }
};

// Functions
std::vector<Area> readCsvFile(const std::string &path);
std::string       convertAreasToJson(const std::vector<Area> &areas);

// Main
int main()
{
  // Read data from csv file
  std::vector<Area> areas = readCsvFile("res/data/Recensamant-2021.csv");
  for (const auto &area : areas) {
    std::cout << area.name << std::endl;
    for (int i = 0; i < ETHNICITY_COUNT; i++) {
      Ethnicity ethnicity = area.ethnicities[i];
      std::cout << static_cast<int>(ethnicity.type)
                << " Barbati: " << ethnicity.sex.male
                << " Femei: " << ethnicity.sex.female
                << " Total: " << ethnicity.sex.getTotal() << std::endl;
    }
  }

  std::cout << "Generated Json:\n" << convertAreasToJson(areas);

  // Generate the html document
  Html::Generator generator("res/statistics-website-template/index.html");
  generator.define("AREAS", Html::ConvertType::JSON,
                   [areas]() { return convertAreasToJson(areas); });
  generator.parseTemplateToFile("./index.html");
  std::cout << "Generated html file" << std::endl;
}

// Function Implementations
std::string convertAreasToJson(const std::vector<Area> &areas)
{
  std::stringstream out;

  out << "`{";
  for (size_t i = 0; i < areas.size(); i++) {
    const Area &area = areas[i];
    out << "\"" << area.getId() << "\"" << ": {"
        << "\"name\": \"" << area.name << "\"" << ','
        << "\"totalPopulation\":" << area.getTotalPopulation() << ','
        << "\"ethnicities\": [";
    for (size_t j = 0; j < ETHNICITY_COUNT; j++) {
      out << "{\"type\": " << static_cast<int>(area.ethnicities[j].type) << ','
          << "\"total\":" << area.ethnicities[j].sex.getTotal() << '}';
      if (j < ETHNICITY_COUNT - 1) {
        out << ',';
      }
    }
    out << ']' << '}';
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
          size_t count                                  = stoi(field);
          areas.back().ethnicities[colIdx - 1].sex.male = count;
        }
        break;
      case State::IN_FEMALE_ROW:
        if (field.size() > 0 && field != "*" && field != "-") {
          size_t count                                    = stoi(field);
          areas.back().ethnicities[colIdx - 1].sex.female = count;
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
