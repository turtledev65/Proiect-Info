#include "generator.hpp"

namespace Html
{
Generator::Generator(const fs::path &templatePath)
{
  if (!fs::exists(templatePath)) {
    throw std::runtime_error("Could not find template path");
  }

  auto file = new std::ifstream(templatePath);
  if (!file->good()) {
    throw std::runtime_error("Something went wrong");
  }
  m_Input        = file;
  m_RelativePath = templatePath.parent_path();
}

std::string Generator::parseTemplate()
{
  bool includedJs = false;

  std::stringstream out;
  std::string       line;
  while (getline(*m_Input, line)) {
    if (line.find("<!-- #include") != std::string::npos) {
      fs::path    filePath     = std::move(getIncludePath(line));
      std::string fileContents = std::move(Utils::getFileContents(filePath));

      if (line.find("_html") != std::string::npos) {
        out << fileContents << '\n';
      } else if (line.find("_css") != std::string::npos) {
        out << "<style>\n" << fileContents << "\n</style>\n";
      } else if (line.find("_js") != std::string::npos) {
        if (!includedJs) {
          out << "<script>\n";
          std::string keyword;
          keyword.reserve(5);
          for (const auto &kv : m_Defines) {
            const std::string &name  = kv.first;
            const Define      &value = kv.second;

            switch (value.type) {
            case DefineType::CONST:
              keyword = "const";
              break;
            case DefineType::LET:
              keyword = "let";
              break;
            case DefineType::VAR:
              keyword = "var";
              break;
            }

            out << keyword << " " << name << " = ";
            switch (value.convertType) {
            case ConvertType::STRING:
              out << '`' << value.convert() << '`';
              break;
            case ConvertType::JSON:
              out << "JSON.parse(" << value.convert() << ")";
              break;
            }

            out << ";\n";
          }
          out << "</script>\n";

          includedJs = true;
        }
        out << "<script>\n" << fileContents << "</script>\n";
      }
    } else if (line.find("<!-- #define") != std::string::npos) {
      std::string name = std::move(getDefineName(line));
      if (m_Defines.find(name) == m_Defines.end()) {
        throw std::runtime_error("Define was not created");
      }

      if (line.find("_var") != std::string::npos) {
        m_Defines[name].type = DefineType::VAR;
      } else if (line.find("_let") != std::string::npos) {
        m_Defines[name].type = DefineType::LET;
      } else if (line.find("_const") != std::string::npos) {
        m_Defines[name].type = DefineType::CONST;
      }
    } else {
      out << line << '\n';
    }
  }

  return out.str();
}

void Generator::parseTemplateToFile(const fs::path &path)
{
  std::ofstream out(path);
  out << parseTemplate();
}

fs::path Generator::getIncludePath(const std::string &line, size_t startPos)
{
  std::string pathStr;
  pathStr.reserve(line.size());

  State state = State::NORMAL;
  for (size_t i = startPos; i < line.size(); i++) {
    if (state == State::QUOTE_CLOSED) {
      break;
    }

    char ch = line[i];
    switch (state) {
    case State::NORMAL:
      if (ch == '"') {
        state = State::QUOTE_OPEN;
      }
      break;
    case State::QUOTE_OPEN:
      if (ch == '"') {
        state = State::QUOTE_CLOSED;
        break;
      }
      pathStr += ch;
      break;
    case State::QUOTE_CLOSED:
    default:
      break;
    }
  }

  fs::path filePath(pathStr);
  if (filePath.is_relative()) {
    filePath = m_RelativePath / fs::relative(filePath);
  }
  return filePath;
}

std::string Generator::getDefineName(const std::string &line, size_t startPos)
{
  std::string out;
  out.reserve(line.size());

  State state = State::NORMAL;
  for (size_t i = startPos; i < line.size(); i++) {
    if (state == State::QUOTE_CLOSED) {
      break;
    }

    char ch = line[i];
    switch (state) {
    case State::NORMAL:
      if (ch == '"') {
        state = State::QUOTE_OPEN;
      }
      break;
    case State::QUOTE_OPEN:
      if (ch == '"') {
        state = State::QUOTE_CLOSED;
        break;
      }
      out += ch;
      break;
    case State::QUOTE_CLOSED:
    default:
      break;
    }
  }

  return out;
}

} // namespace Html
