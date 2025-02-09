#pragma once

#include <functional>
#include <string>
#include <vector>

namespace Cli
{
using Strings = const std::vector<std::string> &;

class BaseCommand
{
public:
  explicit BaseCommand(const std::string &name, const std::string &desc)
      : m_Name(std::move(name)), m_Desc(std::move(desc)) {};
  virtual ~BaseCommand() noexcept = default;

  virtual void Exec(Strings cmdLine) = 0;

  const std::string &GetName() const { return m_Name; }
  const std::string &GetDesc() const { return m_Desc; }

private:
  const std::string m_Name;
  const std::string m_Desc;
};

class Command : public BaseCommand
{
public:
  explicit Command(const std::string &name, const std::string &desc,
                   std::function<void()> func)
      : BaseCommand(name, desc), m_Func(std::move(func))
  {
  }

  void Exec(Strings cmdLine) override;

private:
  std::function<void()> m_Func;
};

class CommandWithArgs : public BaseCommand
{
public:
  explicit CommandWithArgs(const std::string &name, const std::string &desc,
                           std::function<void(Strings)> func)
      : BaseCommand(name, desc), m_Func(std::move(func))
  {
  }

  void Exec(Strings cmdLine) override;

private:
  std::function<void(Strings)> m_Func;
};

} // namespace Cli
