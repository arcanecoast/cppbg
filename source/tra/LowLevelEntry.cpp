#include <cppbg/tra/LowLevelEntry.h>
#include <cppbg/tra/TranslationEntryUtilities.h>

using namespace cppbg_tra;

LowLevelEntry::LowLevelEntry(EntityType ElementType, const string& Content) :
    m_position(-1, -1, -1),
    m_content(Content),
    m_type(ElementType)
{
}

LowLevelEntry::LowLevelEntry(EntityType ElementType, const string& Content,
    int IndexOfContentPlacementLine, int IndexOfContentPlacementInLine,
    int OffsetToContentPlacement) :
    m_position(IndexOfContentPlacementLine, IndexOfContentPlacementInLine, OffsetToContentPlacement),
    m_content(Content),
    m_type(ElementType)
{
}

LowLevelEntry::LowLevelEntry(EntityType ElementType, const string& Content, const Position& Pos) :
    m_position(Pos),
    m_content(Content),
    m_type(ElementType)
{
}

string LowLevelEntry::GetWritableData() const
{
    switch (m_type) {
    case ENTITY_ID:
        return '@' + m_content;
    case ENTITY_TEXT:
        return EscapeTranslationContent(m_content);
    case ENTITY_SOUND:
        return '[' + m_content + ']';
    default:
        return m_content;
    }
}