#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

namespace Csv
{
enum class FieldType {
  DATA,
  ROW_END,
  CSV_END,
};
struct Field {
  explicit Field(FieldType t) : type(t) {}
  explicit Field(const std::string &&str)
      : type(FieldType::DATA), data(std::move(str))
  {
  }

  FieldType   type;
  std::string data;
};

class Reader
{
public:
  Reader(std::istream &input);

  // Functions used for configuration
  Reader &&delimiter(char value)
  {
    m_Delimiter = value;
    return std::move(*this);
  }
  Reader &&quote(char value)
  {
    m_Quote = value;
    return std::move(*this);
  }
  Reader &&newLine(char value)
  {
    m_NewLine = value;
    return std::move(*this);
  }

  // Functions used for parsing
  Field nextField();

  // Iterator implementation, which reads row by row
  class iterator
  {
  public:
    using value_t = std::vector<std::string>;

    explicit iterator(Reader *p, bool end = false);

    iterator      &operator++();
    iterator       operator++(int);
    const value_t &operator*();
    const value_t *operator->();
    bool           operator==(const iterator &other);
    bool           operator!=(const iterator &other);

  private:
    Reader *m_Reader;
    size_t  m_CurrRow = -1;

    static constexpr size_t DEFAULT_ROW_CAPACITY = 50;
    value_t                 m_Row{};

    void next();
  };

  iterator begin() { return iterator(this); };
  iterator end() { return iterator(this, true); };

private:
  // Configurable attributes
  std::istream *m_Input;
  char          m_Delimiter = ';';
  char          m_Quote     = '"';
  char          m_NewLine   = '\n';

  // Buffers
  static constexpr int FIELD_BUFFER_CAPACITY = 1024;
  static constexpr int INPUT_BUFFER_CAPACITY = 1024 * 128;
  std::string          m_FieldBuffer{};
  std::vector<char>    m_InputBuffer = std::vector<char>(INPUT_BUFFER_CAPACITY);

  // Used for reading the buffers
  std::size_t m_Cursor    = 0;
  std::size_t m_BytesRead = 0;
  bool        m_Eof       = false;

  // State of the cursor
  enum class State {
    START_OF_FIELD,
    IN_FIELD,
    IN_QUOTED_FIELD,
    IN_ESCAPED_QUOTE,
    END_OF_ROW,
    END_OF_CSV,
  };
  State m_State = State::START_OF_FIELD;

  char *topToken();
};
} // namespace Csv
