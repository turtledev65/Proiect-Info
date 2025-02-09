#include "app.hpp"

namespace Cli
{
void Command::Exec(Strings cmdLine)
{
  if (cmdLine.empty()) {
    return;
  }
  if (cmdLine[0] != m_Name) {
    return;
  }

  std::vector<std::string> args(cmdLine.begin() + 1, cmdLine.end());
  m_Func(args);
}

void App::Start()
{
  m_Running = true;
  while (m_Running) {
    std::cout << m_Prompt;
    std::string line;
    getline(std::cin, line);
    handleCmdLine(getCmdLine(line));
  }
}

void App::Stop() { m_Running = false; }

void App::AddCommand(const std::string &name, std::function<void(Strings)> func,
                     const std::string &desc)
{
  if (m_Commands.find(name) != m_Commands.end()) {
    return;
  }

  Command cmd(name, func, desc);
  m_Commands.insert({name, cmd});
}

void App::AddHelpCommand(const std::string &name, const std::string &desc)
{
  AddCommand(
      name,
      [&](Strings args) {
        std::cout << m_Name << '\n';
        for (const auto &i : m_Commands) {
          std::cout << std::left << std::setw(20) << i.first
                    << i.second.getDesc() << '\n';
        }
      },
      desc);
}

void App::AddExitCommand(const std::string &name, const std::string &desc)
{
  AddCommand(name, [&](Strings args) { Stop(); }, desc);
}

void App::handleCmdLine(Strings cmdLine)
{
  if (cmdLine.empty()) {
    return;
  }

  auto it = m_Commands.find(cmdLine[0]);
  if (it != m_Commands.end()) {
    it->second.Exec(cmdLine);
  } else {
    std::cout << "wrong command: " << cmdLine[0] << '\n';
  }
}

std::vector<std::string> App::getCmdLine(const std::string &line)
{
  std::vector<std::string> out{};
  std::string              currToken = "";

  bool quoteOpen    = false;
  bool inWhitespace = false;

  for (char ch : line) {
    switch (ch) {
    case ' ':
      if (!inWhitespace && !quoteOpen) {
        inWhitespace = true;
        out.push_back(currToken);
        currToken.clear();
      }
      break;
    case '"':
      if (quoteOpen) {
        quoteOpen = false;
        out.push_back(currToken);
        currToken.clear();
      } else {
        quoteOpen = true;
      }
      break;
    default:
      inWhitespace = false;
      currToken += ch;
      break;
    }
  }
  if (!currToken.empty())
    out.push_back(currToken);

  return out;
}

} // namespace Cli
