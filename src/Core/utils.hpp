#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Utils
{
namespace fs = std::filesystem;

std::string getFileContents(const fs::path &path);
void systemOpenHTMLDoc(const fs::path &path);

} // namespace Utils
