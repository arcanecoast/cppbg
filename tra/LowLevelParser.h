#ifndef CPPBG_TRA_LOWLEVELPARSER 
#define CPPBG_TRA_LOWLEVELPARSER

/**
 * Author: metamaker
 * Creation date: 22.08.2013 14:05
 */

#include <list>
#include <fstream>

#include "LowLevelEntry.h"

using std::list;
using std::ifstream;

namespace cppbg_tra
{
	class LowLevelParser
	{
    public:
        typedef list<LowLevelEntry> container_type;

	public:
		void LoadFromFile(const char* Filename);
		void SaveToFile(const char* Filename) const;

        const container_type& GetItems() const { return m_entries; }
        container_type& GetItems() { return m_entries; }

    private:
        char GetChar(ifstream& in);

    private:
        container_type m_entries;

        int m_currentLineIndex;
        int m_currentLineStart;

        Position m_newElementPosition;
	};
};

#endif // CPPBG_TRA_LOWLEVELPARSER