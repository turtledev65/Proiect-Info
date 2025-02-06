#pragma once

#include <functional>
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "Core/utils.hpp"

namespace Html
{
namespace fs = std::filesystem;

enum class DefineType { CONST = 0, LET, VAR };
enum class ConvertType { STRING = 0, JSON };
struct Define {
  DefineType                   type        = DefineType::CONST;
  ConvertType                  convertType = ConvertType::STRING;
  std::function<std::string()> convert;
};

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
  Generator &&define(const std::string &name, ConvertType convertType,
                     std::function<std::string()> toJson)
  {
    m_Defines[name].convert     = toJson;
    m_Defines[name].convertType = convertType;
    return std::move(*this);
  };

  std::string parseTemplate();
  void        parseTemplateToFile(const fs::path &path);

private:
  // Configurable attributes
  std::istream                           *m_Input;
  fs::path                                m_RelativePath;
  std::unordered_map<std::string, Define> m_Defines;

  // State for parsing the include
  enum class State { NORMAL = 0, QUOTE_OPEN, QUOTE_CLOSED };
  fs::path    getIncludePath(const std::string &line, size_t startPos = 0);
};
} // namespace Html
