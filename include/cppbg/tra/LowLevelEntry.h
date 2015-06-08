#ifndef CPPBG_TRA_LOWLEVELENTRY
#define CPPBG_TRA_LOWLEVELENTRY

/**
 * Author: metamaker
 * Creation date: 30.11.2013 16:05
 */

#include <string>

#include "Position.h"

using std::string;

namespace cppbg_tra
{
	class LowLevelEntry
	{
    public:
        enum EntityType {
            ENTITY_ID    = 0,
            ENTITY_TEXT  = 1,
            ENTITY_SOUND = 2,
            ENTITY_MISC  = 3
        };

	public:
        explicit LowLevelEntry(EntityType EntryType, const string& Content);
        explicit LowLevelEntry(EntityType EntryType, const string& Content,
            int IndexOfContentPlacementLine, int IndexOfContentPlacementInLine,
            int OffsetToContentPlacement);
        explicit LowLevelEntry(EntityType EntryType, const string& Content,
            const Position& Pos);

        void SetContent(const string& Content) { m_content = Content; }

        string GetWritableData() const;

        string GetContent() const { return m_content; }
        EntityType GetType() const { return m_type; }
        Position GetPosition() const { return m_position; }

    private:
        Position                    m_position;
        string                      m_content;
        LowLevelEntry::EntityType   m_type;
	};
};

#endif // CPPBG_TRA_LOWLEVELENTRY