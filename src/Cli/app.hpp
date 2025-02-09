#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <functional>

namespace Cli
{
using Strings = const std::vector<std::string> &;

class Command
{
public:
  Command(const std::string &name, std::function<void(Strings)> func,
          const std::string &desc)
      : m_Name(std::move(name)), m_Func(std::move(func)),
        m_Desc(std::move(desc))
  {
  }
  void Exec(Strings cmdLine);

  const std::string &getDesc() const { return m_Desc; };

private:
  const std::string            m_Name;
  const std::string            m_Desc;
  std::function<void(Strings)> m_Func;
};

class App
{

public:
  App(const std::string &name) : m_Name(std::move(name)) {}

  void Start();
  void Stop();

  // Commands
  void AddCommand(const std::string &name, std::function<void(Strings)> func,
                  const std::string &desc = "");
  void AddHelpCommand(const std::string &name, const std::string &desc = "");
  void AddExitCommand(const std::string &name, const std::string &desc = "");

  // Handlers
  void WrongCmdHandler(std::function<void(const std::string &)> func)
  {
    m_WrongCmdHandler = std::move(func);
  }
  void StartHandler(std::function<void()> f) { m_StartHandler = std::move(f); }
  void StopHandler(std::function<void()> f) { m_StopHandler = std::move(f); }

  // Getters
  const std::string &GetName() { return m_Name; }

private:
  const std::string m_Name    = "";
  std::string       m_Prompt  = "> ";
  bool              m_Running = false;

  std::unordered_map<std::string, Command> m_Commands;

  // Handlers
  std::function<void(const std::string &)> m_WrongCmdHandler;
  std::function<void()>                    m_StartHandler;
  std::function<void()>                    m_StopHandler;

  void                            handleCmdLine(Strings cmdLine);
  static std::vector<std::string> getCmdLine(const std::string &line);
};
} // namespace Cli
