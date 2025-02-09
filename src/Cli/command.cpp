#include "command.hpp"

namespace Cli
{
void Command::Exec(Strings cmdLine)
{
  if (cmdLine.empty()) {
    return;
  }
  if (cmdLine[0] != GetName()) {
    return;
  }

  m_Func();
}

void CommandWithArgs::Exec(Strings cmdLine)
{
  if (cmdLine.empty()) {
    return;
  }
  if (cmdLine[0] != GetName()) {
    return;
  }

  std::vector<std::string> args(cmdLine.begin() + 1, cmdLine.end());
  m_Func(args);
}

} // namespace Cli
