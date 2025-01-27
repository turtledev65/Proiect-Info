#pragma once

#include <iostream>
#include <fstream>
#include <vector>

namespace Csv
{
    enum class FieldType
    {
        DATA,
        ROW_END,
        CSV_END,
    };
    struct Field
    {
        explicit Field(FieldType t): type(t) {}
        explicit Field(const std::string &&str)
            : type(FieldType::DATA), data(std::move(str)) {}
        
        FieldType type;
        std::string data;
    };

    class Parser
    {
        public:
            Parser(std::istream &input);

            Field nextField();

           
        private:
            // Configurable attributes
            std::istream *m_Input;
            char m_Delimiter = ';';
            char m_Quote = '"';
            char m_NewLine = '\n';


            // Buffers
            static constexpr int FIELD_BUFFER_CAPACITY = 1024;
            static constexpr int INPUT_BUFFER_CAPACITY = 1024 * 128;
            std::string m_FieldBuffer {};
            std::vector<char> m_InputBuffer = std::vector<char>(INPUT_BUFFER_CAPACITY); 

            // Used for reading the buffers
            std::size_t m_Cursor = 0;
            std::size_t m_BytesRead = 0;
            bool m_Eof = false;

            // State of the cursor
            enum class State 
            {
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
