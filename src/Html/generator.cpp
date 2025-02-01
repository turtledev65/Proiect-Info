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
  std::stringstream out;
  std::string line;
  while (getline(*m_Input, line)) {
    size_t pos = line.find("<!-- #include");
    if (pos != std::string::npos) {
      fs::path    filePath     = std::move(getIncludePath(line, pos));
      std::string fileContents = std::move(Utils::getFileContents(filePath));

      if (line.find("_html", pos) != std::string::npos) {
        out << fileContents << '\n';
      } else if (line.find("_css", pos) != std::string::npos) {
        out << "<style>\n" << fileContents << "\n</style>\n";
      } else if (line.find("_js", pos) != std::string::npos) {
        out << "<script>\n" << fileContents << "\n</script>\n";
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

} // namespace Html
