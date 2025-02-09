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

std::vector<fs::path>
getDirectoryFiles(const fs::path                &directory,
                  const std::vector<std::string> extensions)
{
  std::vector<fs::path> files{};

  for (const auto &file : fs::directory_iterator(directory)) {
    if (fs::is_regular_file(file)) {
      if (extensions.empty() ||
          std::find(extensions.begin(), extensions.end(),
                    file.path().extension().string()) != extensions.end()) {
        files.push_back(file.path());
      }
    }
  }

  return files;
}
} // namespace Utils
