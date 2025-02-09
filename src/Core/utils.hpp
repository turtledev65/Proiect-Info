#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Utils
{
namespace fs = std::filesystem;

std::string getFileContents(const fs::path &path);
std::vector<fs::path>
getDirectoryFiles(const fs::path                &directory = fs::current_path(),
                  const std::vector<std::string> extensions = {});
} // namespace Utils
