#ifndef CPPBG_TRA_HIGHLEVELPARSER 
#define CPPBG_TRA_HIGHLEVELPARSER

/**
 * Author: metamaker
 * Creation date: 22.08.2013 14:05
 * 
 * Parser for WeiDU .tra files.
 */

#include <map>
#include <boost/shared_ptr.hpp>

#include "TranslationEntry.h"

using boost::shared_ptr;
using std::map;

namespace cppbg_tra
{
    class HighLevelParserImpl;

	class HighLevelParser
	{
    public:
        typedef map<int, shared_ptr<TranslationEntry> > container_type;

    public:
        HighLevelParser();

        void LoadFromFile(const char* Filename);
        void SaveToFile(const char* Filename) const;

        const container_type& GetItems() const;
        container_type& GetItems();

    private:
        shared_ptr<HighLevelParserImpl> pimpl;
	};
};

#endif // CPPBG_TRA_HIGHLEVELPARSER