#ifndef CPPBG_TLK_V1_TALKTABLE
#define CPPBG_TLK_V1_TALKTABLE

/**
 * Author: metamaker
 * Creation date: 30.11.2013 14:28
 */

#include <vector>

#include "TalkTableEntry.h"

using std::vector;

namespace cppbg_tlk_v1
{
    class TalkTable
    {
    public:
        enum ContentLanguageType {
            ENGLISH             = 0,
            FRENCH              = 1,
            GERMAN              = 2,
            ITALIAN             = 3,
            SPANISH             = 4,
            POLISH              = 5,
            KOREAN              = 128,
            CHINESE_TRADITIONAL = 129,
            CHINESE_SIMPLIFIED  = 130,
            JAPANESE            = 131
        };

        typedef vector<TalkTableEntry> container_type;

    private:
        static const long HEADER_SIZE;
        static const long ENTRY_SIZE;
        static const char SIGNATURE[8];

        struct TempTalkTableEntry {
            short         flags;
            long          volumeVariance;
            long          pitchVariance;
            unsigned long offsetToString;
            unsigned long stringSize;
            string        soundResRef;
        };

	    ContentLanguageType m_language;
        container_type m_entries;

    public:
	    TalkTable();

	    void LoadFromFile(const char* Filename);
	    void SaveToFile(const char* Filename) const;

        ContentLanguageType GetLanguage() const { return m_language; }
        void SetLanguage(ContentLanguageType language) { m_language = language; }

        const container_type& GetItems() const { return m_entries; }
        container_type& GetItems() { return m_entries; }

    private:
        long GetContentOffset() const;
    };
};

#endif //CPPBG_TLK_V1_TALKTABLE