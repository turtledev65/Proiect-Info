#pragma once

#include "utils.hpp"

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
} // namespace Utils
