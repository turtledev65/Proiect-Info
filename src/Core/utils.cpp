#pragma once

#include "utils.hpp"
#include <stdexcept>

namespace Utils
{
std::string getFileContents(const fs::path &path)
{
  if (!fs::exists(path)) {
    throw std::runtime_error("Path doesen't exist");
  }

  std::ifstream     file(path);
  std::stringstream out;
  if (!file.good()) {
    throw std::runtime_error("Something went wrong");
  }

  std::string line;
  while (getline(file, line)) {
    out << line << '\n';
  }

  return out.str();
}

void systemOpenHTMLDoc(const fs::path &path)
{
  if (!std::filesystem::exists(path)) {
    return;
  }
  if (std::filesystem::is_directory(path)) {
    return;
  }

  std::string       command;
  std::stringstream stream;
  command.reserve(20);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  command.append("start ");
#elif defined(__linux__)
  command.append("xdg-open ");
#else
#error "Your platform is not supported"
#endif
  command.append(path.string());
  std::system(command.c_str());
}
} // namespace Utils
