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
  void AddCommand(const std::string &name, std::function<void(Strings)> func,
                  const std::string &desc = "");
  void AddHelpCommand(const std::string &name, const std::string &desc = "");
  void AddExitCommand(const std::string &name, const std::string &desc = "");

private:
  std::string m_Name    = "";
  std::string m_Prompt  = "> ";
  bool        m_Running = false;

  std::unordered_map<std::string, Command> m_Commands;

  void                            handleCmdLine(Strings cmdLine);
  static std::vector<std::string> getCmdLine(const std::string &line);
};
} // namespace Cli
