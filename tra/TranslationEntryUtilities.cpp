#include "TranslationEntryUtilities.h"

string cppbg_tra::EscapeTranslationContent(const string& content)
{
    string boundary;

    bool hasTilda   = content.find('~') != content.npos,
         hasPercent = content.find('%') != content.npos,
         hasQuote   = content.find('"') != content.npos;

    if (!hasTilda) {
        boundary = '~';
    } else if (!hasQuote) {
        boundary = '"';
    } else if (!hasPercent) {
        boundary = '%';
    } else {
        boundary = "~~~~~";
    }

    return boundary + content + boundary;
}