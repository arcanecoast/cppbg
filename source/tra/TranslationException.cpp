#include <cppbg/tra/TranslationException.h>

using namespace cppbg_tra;

TranslationException::TranslationException() :
    m_indexOfBadLine(-1),
    m_indexOfBadCharacterInLine(-1),
    m_offsetToBadCharacter(-1)
{
}

TranslationException::TranslationException(int IndexOfBadLine, int IndexOfBadCharacterInLine, int OffsetToBadCharacter) :
    m_indexOfBadLine(IndexOfBadLine),
    m_indexOfBadCharacterInLine(IndexOfBadCharacterInLine),
    m_offsetToBadCharacter(OffsetToBadCharacter)
{
}

TranslationException::TranslationException(const Position& Pos) :
    m_indexOfBadLine(Pos.GetIndexOfLine()),
    m_indexOfBadCharacterInLine(Pos.GetPlaceInLine()),
    m_offsetToBadCharacter(Pos.GetPhysicalOffset())
{
}