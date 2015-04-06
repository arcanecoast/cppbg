#ifndef CPPBG_TRA_HIGHLEVELPARSERIMPL 
#define CPPBG_TRA_HIGHLEVELPARSERIMPL

/**
 * Author: metamaker
 * Creation date: 22.08.2013 14:05
 * 
 * Parser for WeiDU .tra files.
 */

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "LowLevelParser.h"
#include "TranslationEntry.h"

using std::string;
using std::map;
using std::vector;
using boost::shared_ptr;

namespace cppbg_tra
{
	class HighLevelParser
	{
    public:
        typedef map<int, shared_ptr<TranslationEntry> > container_type;

    public:
        void LoadFromFile(const char* Filename);
        void SaveToFile(const char* Filename) const;

        const container_type& GetItems() const { return m_entries; }
        container_type& GetItems() { return m_entries; }

    private:
        struct LinkToEntry {
            int id;
            int idAssignedTo;
            LowLevelParser::container_type::iterator referenceToAssignee;

            LinkToEntry(int Id, int IdAssignedTo, LowLevelParser::container_type::iterator ReferenceToAssignee) : 
                id(Id), idAssignedTo(IdAssignedTo), referenceToAssignee(ReferenceToAssignee) {}
        };

    private:
        void CheckStack(LowLevelParser::container_type::iterator newElement, vector<LowLevelParser::container_type::iterator>& stack);
        void AggregateEntry(int EntryID, vector<LowLevelParser::container_type::iterator>& stack);
        int  GetIntID(LowLevelParser::container_type::iterator element);

    private:
        container_type m_entries;
	};
};

#endif // CPPBG_TRA_HIGHLEVELPARSERIMPL