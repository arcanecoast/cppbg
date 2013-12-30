#include "TranslationEntryUtilities.h"

string cppbg_tra::escape_content(const string& Content)
{
    bool HasTildas   = Content.find('~') != string::npos;
    bool HasPercents = Content.find('%') != string::npos;
    bool HasQuotes   = Content.find('"') != string::npos;

    if (HasTildas && HasPercents && HasQuotes) {
        return "~~~~~" + Content + "~~~~~";
    } else if (!HasTildas) {
        return '~' + Content + '~';
    } else if (!HasQuotes){
        return '"' + Content + '"';
    }

    return '%' + Content + '%';
}