#include <fstream>

#include "SyntaxError.h"
#include "LowLevelParser.h"

using namespace cppbg_tra;
using namespace std;

char LowLevelParser::GetChar(ifstream& in)
{
    char c = in.get();

    if (c == '\n') {
        m_currentLineIndex++;
        m_currentLineStart = in.tellg();
    }

    return c;
}

void LowLevelParser::LoadFromFile(const char* Filename)
{
    m_entries.clear();

    ifstream in;
    in.open(Filename, ios_base::in);
    if (!in.good()) {
        throw ios_base::failure("Unable to open file for reading!");
    }

    m_currentLineIndex = 0;
    m_currentLineStart = 0;

    bool mustBeEqualnessSymbol = false;
    bool previousValuableEntityIsID = false;

    string junk;

    for (;;) {
        char c = GetChar(in);

        if (!in.good()) {
            break;
        }

        switch (c) {
        case '@':
        case '[':
        case '~':
        case '%':
        case '"':
            int currentOffset = in.tellg();

            if (mustBeEqualnessSymbol) {
                throw SyntaxError(SyntaxError::ERROR_EQUALENESS_SYMBOL_IS_MISSING, m_currentLineIndex, currentOffset - m_currentLineStart, currentOffset);
            }

            if (!junk.empty()) {
                m_entries.push_back(LowLevelEntry(LowLevelEntry::ENTITY_MISC, junk, m_newElementPosition));
                junk.clear();
            }

            m_newElementPosition.Update(m_currentLineIndex, currentOffset - m_currentLineStart, currentOffset);
        };

        switch (c) {
        case '@':
            {
                string content;
                for (;;) {
                    c = GetChar(in);
                    if (!in.good()) {
                        break;
                    }

                    if (isdigit(c) || (content.empty() && c == '-')) {
                        content += c;
                    } else {
                        break;
                    }
                }

                if (content.empty() || content.length() > 11) {
                    throw SyntaxError(SyntaxError::ERROR_BAD_INDEX, m_newElementPosition);
                }

                m_entries.push_back(LowLevelEntry(LowLevelEntry::ENTITY_ID, content,m_newElementPosition));

                int currentOffset = (int)in.tellg() - 1;
                m_newElementPosition.Update(m_currentLineIndex, currentOffset - m_currentLineStart, currentOffset);

                if (c != '=' && c != ' ' && c != '\n' && c != '\t' && in.good()) {
                    throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_SYMBOL, m_newElementPosition);
                }

                if (!previousValuableEntityIsID) {
                    mustBeEqualnessSymbol      = c != '=';
                    previousValuableEntityIsID = true;
                } else {
                    mustBeEqualnessSymbol      = false;
                    previousValuableEntityIsID = false; // Because schema @1 = @2 = @3 is impossible - this is appropriate
                }

                if (in.good()) {
                    junk += c;
                }
            }
            break;
        case '[':
            {
                string content;
                for (;;) {
                    c = GetChar(in);
                    if (!in.good()) {
                        throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_END_OF_FILE);
                    }

                    if (c != ']') {
                        content += c;
                    } else {
                        break;
                    }
                }

                m_entries.push_back(LowLevelEntry(LowLevelEntry::ENTITY_SOUND, content, m_newElementPosition));

                int currentOffset = in.tellg();
                m_newElementPosition.Update(m_currentLineIndex, currentOffset - m_currentLineStart, currentOffset);

                previousValuableEntityIsID = false;
            }
            break;
        case '~':
        case '%':
        case '"':
            {
                string boundary_symbol, content;
                boundary_symbol += c;

                int last_entry_line       = m_currentLineIndex,
                    last_entry_line_start = m_currentLineStart;

                c = GetChar(in);
                if (boundary_symbol[0] == '~' && c == '~') {
                    boundary_symbol += c;

                    for (int i = 0; i < 3; ++i) {
                        c = GetChar(in);

                        if (c != '~') {
                            in.unget();

                            if (c == '\n') {
                                m_currentLineIndex--;
                                m_currentLineStart--;
                            }

                            break;
                        }

                        if (in.good()) {
                            boundary_symbol += c;
                        } else {
                            break;
                        }
                    }

                    if (boundary_symbol != "~~~~~") {
                        // Then, boundary symbol is ~
                        for (int i = 1; i < boundary_symbol.length(); ++i) {
                            in.unget();
                        }

                        boundary_symbol = '~';
                    } else if (boundary_symbol == "\"\""
                        || boundary_symbol == "~~"
                        || boundary_symbol == "%%") {
                        boundary_symbol = boundary_symbol[0];
                        content += boundary_symbol[0];
                    } else if (!in.good()) {
                        throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_END_OF_FILE);
                    }
                } else {
                    content += c;
                }

                int boundary_symbol_length = boundary_symbol.length();

                while (content.length() < boundary_symbol_length || content.substr(content.length() - boundary_symbol_length, boundary_symbol_length) != boundary_symbol) {
                    c = GetChar(in);
                    content += c;

                    if (!in.good()) {
                        throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_END_OF_FILE, last_entry_line, 0, m_currentLineStart);
                    }
                }
           
                content = content.substr(0, content.length() - boundary_symbol_length);
                m_entries.push_back(LowLevelEntry(LowLevelEntry::ENTITY_TEXT, content, m_newElementPosition));

                int currentOffset = in.tellg();
                m_newElementPosition.Update(m_currentLineIndex, currentOffset - m_currentLineStart, currentOffset);

                previousValuableEntityIsID = false;
            }
            break;
        case '/':
            junk += c;

            c = GetChar(in);
            junk += c;

            if (c == '*') {
                // Multi-line comment

                for (;;) {
                    c = GetChar(in);
                    junk += c;

                    if (c == '*') {
                        c = GetChar(in);
                        junk += c;

                        if (c == '/') {
                            break;
                        }
                    }

                    if (!in.good()) {
                        throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_END_OF_FILE);
                    }
                }
            } else if (c == '/') {
                // One-line comment

                for (;;) {
                    c = GetChar(in);
                    junk += c;

                    if (c == '\n') {
                        junk += c;
                        break;
                    }

                    if (!in.good()) {
                        break;
                    }
                }
            } else {
                int currentOffset = in.tellg();
                throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_SYMBOL, m_currentLineIndex, currentOffset - m_currentLineStart, currentOffset);
            }
            break;
        case '\n':
        case ' ':
        case '\t':
            junk += c;
            break;
        case '=':
            if (mustBeEqualnessSymbol) {
                junk += c;
                mustBeEqualnessSymbol = false;
                break;
            }
        default:
            {
                int currentOffset = in.tellg();
                throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_SYMBOL, m_currentLineIndex, currentOffset - m_currentLineStart, currentOffset);
            }
            break;
        };
    }

    if (mustBeEqualnessSymbol) {
        throw SyntaxError(SyntaxError::ERROR_UNEXPECTED_END_OF_FILE);
    }

    if (!junk.empty()) {
        m_entries.push_back(LowLevelEntry(LowLevelEntry::ENTITY_MISC, junk, m_newElementPosition));
        junk.clear();
    }
}

void LowLevelParser::SaveToFile(const char* Filename) const
{
    ofstream out;
    out.open(Filename, ios_base::out | ios_base::trunc);
    if (!out.good()) {
        throw ios_base::failure("Unable to open file for writing!");
    }

    for (container_type::const_iterator i = m_entries.begin(); i != m_entries.end(); ++i) {
        out << i->GetWritableData();
    }
}