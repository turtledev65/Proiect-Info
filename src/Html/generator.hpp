#pragma once

#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Core/utils.hpp"

namespace Html
{
namespace fs = std::filesystem;

class Generator
{
public:
  Generator(std::istream &input) : m_Input(&input) {};
  Generator(const fs::path &templatePath);
  ~Generator() { delete m_Input; };

  Generator &&relativePath(const fs::path &path)
  {
    m_RelativePath = std::move(path);
    return std::move(*this);
  };

  std::string parseTemplate();
  void        parseTemplateToFile(const fs::path &path);

private:
  // Configurable attributes
  std::istream *m_Input;
  fs::path      m_RelativePath;

  // State for parsing the include
  enum class State { NORMAL = 0, QUOTE_OPEN, QUOTE_CLOSED };
  fs::path getIncludePath(const std::string &line, size_t startPos = 0);
};
} // namespace Html
