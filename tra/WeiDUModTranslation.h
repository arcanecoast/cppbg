#ifndef CPPBG_TRA_WEIDU_MOD_TRANSLATION
#define CPPBG_TRA_WEIDU_MOD_TRANSLATION

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
    class HighLevelParser;

	class WeiDUModTranslation
	{
    public:
        typedef map<int, shared_ptr<TranslationEntry> >::iterator iterator;
        typedef map<int, shared_ptr<TranslationEntry> >::const_iterator const_iterator;
        typedef map<int, shared_ptr<TranslationEntry> >::reverse_iterator reverse_iterator;
        typedef map<int, shared_ptr<TranslationEntry> >::const_reverse_iterator const_reverse_iterator;

    public:
        WeiDUModTranslation();

        void LoadFromFile(const char* Filename);
        void SaveToFile(const char* Filename) const;

        const_iterator Begin() const;
        iterator Begin();

        const_iterator End() const;
        iterator End();

        const_reverse_iterator Rbegin() const;
        reverse_iterator Rbegin();

        const_reverse_iterator Rend() const;
        reverse_iterator Rend();

        const_iterator Get(int id) const;
        iterator Get(int id);

        const_iterator operator[](int id) const;
        iterator operator[](int id);

        bool Insert(const TranslationEntry& entry);
        size_t Remove(int id);

        size_t Size() const;
        bool Empty() const;

    private:
        shared_ptr<HighLevelParser> pimpl;
	};
};

#endif // CPPBG_TRA_WEIDU_MOD_TRANSLATION