#include "app.hpp"

namespace Cli
{
void App::Start()
{
  if (m_StartHandler) {
    m_StartHandler();
  }

  m_Running = true;
  while (m_Running) {
    std::cout << m_Prompt;
    std::string line;
    getline(std::cin, line);
    handleCmdLine(getCmdLine(line));
  }
}

void App::Stop()
{
  if (m_StopHandler) {
    m_StopHandler();
  }

  m_Running = false;
}

void App::AddCommand(const std::string &name, std::function<void()> func,
                     const std::string &desc)
{
  if (m_Commands.find(name) != m_Commands.end()) {
    return;
  }

  m_Commands.insert({name, std::make_shared<Command>(name, desc, func)});
}

void App::AddCommand(const std::string &name, std::function<void(Strings)> func,
                     const std::string &desc)
{
  if (m_Commands.find(name) != m_Commands.end()) {
    return;
  }

  m_Commands.insert(
      {name, std::make_shared<CommandWithArgs>(name, desc, func)});
}

void App::AddHelpCommand(const std::string &name, const std::string &desc)
{
  AddCommand(
      name,
      [&]() {
        std::cout << m_Name << '\n';
        for (const auto &i : m_Commands) {
          std::cout << std::left << std::setw(20) << i.first
                    << i.second->GetDesc() << '\n';
        }
      },
      desc);
}

void App::AddExitCommand(const std::string &name, const std::string &desc)
{
  AddCommand(name, [&]() { Stop(); }, desc);
}

void App::handleCmdLine(Strings cmdLine)
{
  if (cmdLine.empty()) {
    return;
  }

  auto it = m_Commands.find(cmdLine[0]);
  if (it != m_Commands.end()) {
    it->second->Exec(cmdLine);
  } else if (m_WrongCmdHandler) {
    m_WrongCmdHandler(cmdLine[0]);
  } else {
    std::cout << "wrong command: " << cmdLine[0] << '\n';
  }
}

std::vector<std::string> App::getCmdLine(const std::string &line)
{
  enum class State {
    NORMAL,
    WHITESPACE,
    QUOTE_OPEN,
  };
  State state = State::NORMAL;

  std::vector<std::string> out{};
  std::string              tokenBuffer = "";
  char                     quote       = '"';

  for (char ch : line) {
    switch (state) {
    case State::NORMAL:
      switch (ch) {
      case '"':
      case '\'':
        quote = ch;
        state = State::QUOTE_OPEN;
        break;
      case ' ':
        state = State::WHITESPACE;
        if (!tokenBuffer.empty()) {
          out.push_back(tokenBuffer);
          tokenBuffer.clear();
        }
        break;
      default:
        tokenBuffer += ch;
        break;
      }
      break;
    case State::WHITESPACE:
      switch (ch) {
      case '"':
      case '\'':
        quote = ch;
        state = State::QUOTE_OPEN;
        break;
      case ' ':
        break;
      default:
        tokenBuffer += ch;
        state = State::NORMAL;
        break;
      }
      break;
    case State::QUOTE_OPEN:
      if (ch == quote) {
        state = State::NORMAL;
        if (!tokenBuffer.empty()) {
          out.push_back(tokenBuffer);
          tokenBuffer.clear();
        }
        break;
      }
      tokenBuffer += ch;
      break;
    }
  }
  if (!tokenBuffer.empty()) {
    out.push_back(tokenBuffer);
    tokenBuffer.clear();
  }

  return out;
}

} // namespace Cli
