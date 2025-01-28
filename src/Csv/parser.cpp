#include "parser.hpp"

namespace Csv
{
Parser::Parser(std::istream &input) : m_Input(&input)
{
  if (!m_Input->good()) {
    throw std::runtime_error("Something is wrong with the input stream");
  }
  m_FieldBuffer.reserve(FIELD_BUFFER_CAPACITY);

  // Read the input stream
  m_Input->read(m_InputBuffer.data(), INPUT_BUFFER_CAPACITY);
  m_BytesRead = static_cast<size_t>(m_Input->gcount());
  m_Eof       = m_Input->eof();
}

Field Parser::nextField()
{
  if (m_State == State::END_OF_CSV) {
    return Field(FieldType::CSV_END);
  }
  m_FieldBuffer.clear();

  for (;;) {
    char *token = topToken();
    if (token == nullptr) {
      m_State = State::END_OF_CSV;
      return m_FieldBuffer.empty() ? Field(FieldType::CSV_END)
                                   : Field(std::move(m_FieldBuffer));
    }

    char ch = *token;
    switch (m_State) {
    case State::START_OF_FIELD:
      m_Cursor++;
      if (ch == m_NewLine) {
        m_State = State::END_OF_ROW;
        return Field(std::move(m_FieldBuffer));
      }

      if (ch == m_Quote) {
        m_State = State::IN_QUOTED_FIELD;
      } else if (ch == m_Delimiter) {
        return Field(std::move(m_FieldBuffer));
      } else {
        m_State = State::IN_FIELD;
        m_FieldBuffer += ch;
      }

      break;

    case State::IN_FIELD:
      m_Cursor++;
      if (ch == m_NewLine) {
        m_State = State::END_OF_ROW;
        return Field(std::move(m_FieldBuffer));
      }
      if (ch == m_Delimiter) {
        m_State = State::START_OF_FIELD;
        return Field(std::move(m_FieldBuffer));
      }

      m_FieldBuffer += ch;
      break;

    case State::IN_QUOTED_FIELD:
      m_Cursor++;
      if (ch == m_Quote) {
        m_State = State::IN_ESCAPED_QUOTE;
      } else {
        m_FieldBuffer += ch;
      }

      break;

    case State::IN_ESCAPED_QUOTE:
      m_Cursor++;
      if (ch == m_NewLine) {
        m_State = State::END_OF_ROW;
        return Field(std::move(m_FieldBuffer));
      }

      if (ch == m_Quote) {
        m_State = State::IN_QUOTED_FIELD;
        m_FieldBuffer += ch;
      } else if (ch == m_Delimiter) {
        m_State = State::START_OF_FIELD;
        return Field(std::move(m_FieldBuffer));
      } else {
        m_State = State::IN_FIELD;
        m_FieldBuffer += ch;
      }

      break;

    case State::END_OF_ROW:
      m_State = State::START_OF_FIELD;
      return Field(FieldType::ROW_END);

    case State::END_OF_CSV:
      throw std::logic_error("Something went wrong");
    }
  }
}

char *Parser::topToken()
{
  if (m_Cursor == m_BytesRead && m_Eof) {
    return nullptr;
  }

  return &m_InputBuffer[m_Cursor];
}

// Iterator implementation
using iterator = Parser::iterator;

iterator::iterator(Parser *p, bool end) : m_Parser(p)
{
  if (!end) {
    m_Row.reserve(DEFAULT_ROW_CAPACITY);
    m_CurrRow = 0;
    next();
  }
}

iterator &iterator::operator++()
{
  next();
  return *this;
}
iterator iterator::operator++(int)
{
  iterator i = (*this);
  ++(*this);
  return i;
}
const iterator::value_t &iterator::operator*()
{
  return m_Row;
};
const iterator::value_t *iterator::operator->()
{
  return &m_Row;
}
bool iterator::operator==(const iterator &other)
{
  return m_CurrRow == other.m_CurrRow && m_Row.size() == other.m_Row.size();
}
bool iterator::operator!=(const iterator &other)
{
  return !(*this == other);
}

void iterator::next()
{
  size_t numFields = 0;
  for (;;) {
    Field field = m_Parser->nextField();
    switch (field.type) {
    case FieldType::DATA:
      if (numFields < m_Row.size()) {
        m_Row[numFields] = std::move(field.data);
      } else {
        m_Row.push_back(std::move(field.data));
      }
      numFields++;
      continue;
    case FieldType::ROW_END:
      if (numFields < m_Row.size()) {
        m_Row.resize(numFields);
      }
      m_CurrRow++;
      return;
    case FieldType::CSV_END:
      if (numFields < m_Row.size()) {
        m_Row.resize(numFields);
      }
      m_CurrRow = -1;
      return;
    }
  }
}

} // namespace Csv
