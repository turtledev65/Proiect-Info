#include "Csv/reader.hpp"
#include "Html/generator.hpp"
#include "Statistics/area.hpp"
#include "Core/utils.hpp"
#include "Cli/app.hpp"
#include "Chart/chart.hpp"

#include <fstream>

using namespace std;
using namespace Statistics;
using namespace Utils;
using namespace Chart;
namespace fs = std::filesystem;

// Subprograme
std::vector<fs::path> gasesteFisiereleCSV();
void                  listeazaFisiere();
void                  genereazaDocumentHTML(const std::vector<Area> &zone,
                                            const fs::path          &sablon,
                                            const fs::path          &fisierGenerat);

// Main
int main()
{
  vector<Area> zone;

  Cli::App app("Recensamant Romania");

  app.StartHandler([&app]() {
    cout << app.GetName() << '\n'
         << "Foloseste ajutor pentru a vedea toate comenzile\n";
  });
  app.WrongCmdHandler(
      [](const string &cmd) { cout << "Comanda gresita: " << cmd << '\n'; });

  app.AddHelpCommand("ajutor", "Vezi o lista cu toate comenzile si ce fac");
  app.AddExitCommand("iesi", "Inchide programul");

  app.AddCommand("ls", listeazaFisiere,
                 "Listeaza toate fisierele CSV gasite, care contin statistici");
  app.AddCommand(
      "sel",
      [&zone, &app](const vector<string> &args) {
        if (args.empty()) {
          cout << "ID-ul trebuie specificat\n";
          return;
        }

        size_t id;
        try {
          id = stoi(args[0]);

          vector<fs::path> fisiere = gasesteFisiereleCSV();
          if (id >= fisiere.size()) {
            cout << id << " este un ID invalid.\n";
            return;
          }
          const fs::path &fisier = fisiere[id];

          zone = parseCSV(fisier);
          if (zone.empty()) {
            cout << "Nu s-a putu citi fisierul\n";
          } else {
            cout << "Fisierul a fost citit cu succes.\n";
            app.SetPrompt(fisier.filename().string() + " > ");
          }
        } catch (const std::invalid_argument &) {
          cout << "ID invalid: " << id << '\n';
        } catch (const std::out_of_range &) {
          cout << "ID-ul este prea mare: " << args[0] << "\n";
        } catch (...) {
          cout << "Eroare necunoscuta la procesarea ID-ului\n";
        }
      },
      "Selecteaza un fisier cu statistici folosind ID-ul acestuia");
  app.AddCommand(
      "inapoi",
      [&zone, &app] {
        if (zone.empty()) {
          cout << "Niciun fisier selectat\n";
          return;
        }

        zone.clear();
        app.SetPrompt("> ");
      },
      "Du-te inapoi daca ai selectat un fisier");

  app.AddCommand(
      "stat-etnie",
      [&zone](const vector<string> &args) {
        if (zone.empty()) {
          std::cout << "Niciun fisier selectat\n";
          return;
        }

        Ethnicity const *ethnicities = nullptr;
        if (args.empty()) {
          ethnicities = zone[0].m_Ethnicities;
        } else {
          const std::string &name = args[0];
          for (const Area &zona : zone) {
            if (name == zona.GetName()) {
              ethnicities = zona.m_Ethnicities;
              break;
            }
          }
          if (ethnicities == nullptr) {
            std::cout << name << " nu a fost gasit\n";
            return;
          }
        }

        std::vector<ChartItem> items;
        items.reserve(ETHNICITY_COUNT);
        for (size_t i = 1; i < ETHNICITY_COUNT; i++) {
          items.emplace_back(ethnicities[i].sex.GetTotal(),
                             ethnicities[i].GetName());
        }

        BarChart chart(items);
        chart.Print();
      },
      "Afiseaza statisticile pe etnie pentru fisierul selectat. Poti specifica un judet folosind LITERE_MARI");
  app.AddCommand("stat-sex", [&zone](const vector<string> &args) {
    if (zone.empty()) {
      std::cout << "Niciun fisier selectat\n";
      return;
    }

    unique_ptr<Sex> s = nullptr;
    if (args.empty()) {
      s = make_unique<Sex>(zone[0].GetSexTotal());
    } else {
      const std::string &name = args[0];
      for (const Area &zona : zone) {
        if (name == zona.GetName()) {
          s = make_unique<Sex>(zona.GetSexTotal());
          break;
        }
      }
      if (!s) {
        cout << name << " nu a fost gasit\n";
        return;
      }
    }

    std::vector<ChartItem> items{ChartItem(s->male, "Barbati"),
                                 ChartItem(s->female, "Femei")};
    PieChart               chart(items);
    chart.Print();
  }, "Afiseaza statisticile pe etnie pentru fisierul selectat. Poti specifica un judet folosind LITERE_MARI");
  app.AddCommand(
      "gen",
      [&zone]() {
        if (zone.empty()) {
          cout << "Niciun fisier selectat\n";
          return;
        }
        genereazaDocumentHTML(zone,
                              "./res/statistics-website-template/index.html",
                              "./statistici.html");
      },
      "Genereaza documentul HTLM pentru fisierul cu statistici selectat");

  app.Start();

  return 0;
}

// Implementarea subprogramelor
std::vector<fs::path> gasesteFisiereleCSV()
{
  fs::path rootPath = fs::current_path().append("res").append("data");
  if (!fs::exists(rootPath)) {
    cout << "Could not find data folder\n";
  }

  vector<fs::path> files = getDirectoryFiles(rootPath, {".csv"});
  return files;
}

void listeazaFisiere()
{
  vector<fs::path> files = gasesteFisiereleCSV();
  for (size_t i = 0; i < files.size(); i++) {
    cout << '[' << i << ']' << ' ' << files[i].filename().string() << '\n';
  }
}

void genereazaDocumentHTML(const std::vector<Area> &zone,
                           const fs::path          &sablon,
                           const fs::path          &fisierGenerat)
{
  try {
    Html::Generator generator(sablon);
    generator.define("AREAS", Html::ConvertType::JSON, [zone]() {
      stringstream out;

      out << "`{";
      for (size_t i = 0; i < zone.size(); i++) {
        const Area &zona = zone[i];
        out << zona.ToJSON();
        if (i < zone.size() - 1) {
          out << ',';
        }
      }
      out << "}`";

      return out.str();
    });
    generator.parseTemplateToFile(fisierGenerat);

    std::cout << "Fisier HTML generat in: \n"
              << fs::absolute(fisierGenerat).string() << '\n';
  } catch (const std::exception &e) {
    std::cout << "Fisierul HTML nu a putut fi generate\n" << '\t' << e.what();
  }
}
