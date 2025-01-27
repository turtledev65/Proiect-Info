#include "parser.hpp"

namespace Csv
{
    Parser::Parser(std::istream &input): m_Input(&input)
    {
        if (!m_Input->good()) {
            throw std::runtime_error("Something is wrong with the input stream");
        }
        m_FieldBuffer.reserve(FIELD_BUFFER_CAPACITY);

        // Read the input stream
        m_Input->read(m_InputBuffer.data(), INPUT_BUFFER_CAPACITY);
        m_BytesRead = static_cast<size_t>(m_Input->gcount());
        m_Eof = m_Input->eof();
    }

    Field Parser::nextField()
    {
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
                    if (ch == m_NewLine) {
                        m_State = State::END_OF_ROW;
                    }                    

                    if (ch == m_Quote) {
                        m_State = State::IN_QUOTED_FIELD;
                    } else if (ch == m_Delimiter) {
                        return Field(std::move(m_FieldBuffer));
                    } else {
                        m_State = State::IN_FIELD;
                        m_FieldBuffer += ch;
                    }

                    m_Cursor ++;
                    break;
                
                case State::IN_FIELD:
                    if (ch == m_NewLine) {
                        m_State = State::END_OF_ROW;
                        return Field(std::move(m_FieldBuffer));
                    }
                    if (ch == m_Delimiter) {
                        m_State = State::START_OF_FIELD;
                        return Field(std::move(m_FieldBuffer));
                    }
                
                    m_FieldBuffer += ch;
                    m_Cursor ++;
                    break;
                
                case State::IN_QUOTED_FIELD:
                    if (ch == m_Quote) {
                        m_State = State::IN_ESCAPED_QUOTE;
                    } else {
                        m_FieldBuffer += ch;
                    }
                
                    m_Cursor ++;
                    break;
                
                case State::IN_ESCAPED_QUOTE:
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

                    m_Cursor ++;
                    break;
                
                case State::END_OF_ROW:
                    m_State = State::START_OF_FIELD;
                    return Field(FieldType::ROW_END);
                
                case State::END_OF_CSV:
                    throw std::runtime_error("Something went wrong");
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
} // namespace Csv
