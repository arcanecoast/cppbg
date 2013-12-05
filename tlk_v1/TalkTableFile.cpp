#include <fstream>
#include <cstring>
#include <stdexcept>

#include "TalkTableFile.h"

using namespace cppbg_tlk_v1;
using namespace std;

const long TalkTable::HEADER_SIZE  = 18;
const long TalkTable::ENTRY_SIZE   = 26;
const char TalkTable::SIGNATURE[8] = {'T','L','K',' ','V','1',' ',' '};

TalkTable::TalkTable() : 
    m_language(ENGLISH)
{
}

long TalkTable::GetContentOffset() const
{
	return ENTRY_SIZE * m_entries.size() + HEADER_SIZE;
}

void TalkTable::LoadFromFile(const char* Filename)
{
    ifstream in;
    in.open(Filename, ios_base::in | ios::binary);
    if (!in.good()) {
        throw ios_base::failure("Unable to open file for reading!");
    }

    char signature[8];
    in.read(signature, 8);

    if (strncmp(signature, SIGNATURE, 8) != 0) {
        throw logic_error("Not a dialog TLK V1 file");
    }

	unsigned long entriesCount = 0;
	long  contentOffset = 0;
	short languageID = 0;

    m_language = ENGLISH;

    in.read(reinterpret_cast<char*>(&m_language), 2);
    in.read(reinterpret_cast<char*>(&entriesCount), 4);
    in.read(reinterpret_cast<char*>(&contentOffset), 4);

    TempTalkTableEntry tempEntry;
    vector<TempTalkTableEntry> tempEntries;

    m_entries.clear();
    m_entries.reserve(entriesCount);
    tempEntries.reserve(entriesCount);

    char soundResRef[9] = {0,0,0,0,0,0,0,0,0};

	for (unsigned long i = 0; i < entriesCount; i++)
	{
        in.read(reinterpret_cast<char*>(&tempEntry.flags), 2);
        in.read(soundResRef, 8);
        in.read(reinterpret_cast<char*>(&tempEntry.volumeVariance), 4);
        in.read(reinterpret_cast<char*>(&tempEntry.pitchVariance), 4);
        in.read(reinterpret_cast<char*>(&tempEntry.offsetToString), 4);
        in.read(reinterpret_cast<char*>(&tempEntry.stringSize), 4);

        tempEntry.soundResRef = soundResRef;

        tempEntries.push_back(tempEntry);
	}

    string text;
    char*  raw_text;

	for (unsigned long i = 0; i < entriesCount; i++)
	{
        TempTalkTableEntry& tempEntry = tempEntries[i];

		if (tempEntry.stringSize > 0) {
            raw_text = new char[tempEntry.stringSize + 1];
            in.seekg(contentOffset + tempEntry.offsetToString);
            in.read(raw_text, tempEntry.stringSize);
			raw_text[tempEntry.stringSize] = 0;

            text = raw_text;

            delete[] raw_text;
        } else {
            text.clear();
        }

        m_entries.push_back(TalkTableEntry(tempEntry.flags, tempEntry.volumeVariance,
            tempEntry.pitchVariance, tempEntry.soundResRef, text));
	}
}

void TalkTable::SaveToFile(const char * Filename) const
{
	/*FILE * fOut = fopen(Filename, "wb");
	if (!fOut) return false;

	char Sign[] = "TLK V1  ";
	fwrite(Sign, sizeof(char), 8, fOut);

	fwrite(&this->LanguageID, sizeof(uint16), 1, fOut);
	fwrite(&this->uItemsCount, sizeof(uint32), 1, fOut);

	uint32 uContentOffset = GetContentOffset();
	fwrite(&uContentOffset, sizeof(uint32), 1, fOut);

	const uint32 uNullValue = 0;
	uint32 bufValue = 0;
	for (uint32 i = 0; i < this->uItemsCount; i++)
	{
		GotoItm(i);

		uint16 uFlags = ((CTLKItemV1 *)this->Items->Content)->Flags;
		if (uFlags == 0)
		{
			if (((CTLKItemV1 *)this->Items->Content)->Text)
				uFlags = SetBit16(uFlags, 0, true);
			if (((CTLKItemV1 *)this->Items->Content)->SoundResRef[0] != '\0')
				uFlags = SetBit16(uFlags, 1, true);
		}
			
		fwrite(&uFlags, sizeof(uint16), 1, fOut);
		fwrite(&((CTLKItemV1 *)this->Items->Content)->SoundResRef, sizeof(char), 8, fOut);
		fwrite(&((CTLKItemV1 *)this->Items->Content)->VolumeVariance, sizeof(uint32), 1, fOut);
		fwrite(&((CTLKItemV1 *)this->Items->Content)->PitchVariance, sizeof(uint32), 1, fOut);

		if (((CTLKItemV1 *)this->Items->Content)->Text)
		{
			uint32 uLen = strlen(((CTLKItemV1 *)this->Items->Content)->Text);
			fwrite(&bufValue, sizeof(uint32), 1, fOut);
			fwrite(&uLen, sizeof(uint32), 1, fOut);
			bufValue += uLen;
		}
		else
		{
			fwrite(&uNullValue, sizeof(uint32), 1, fOut);
			fwrite(&uNullValue, sizeof(uint32), 1, fOut);
		}
	}

	for (uint32 i = 0; i < this->uItemsCount; i++)
	{
		GotoItm(i);

		if (((CTLKItemV1 *)this->Items->Content)->Text)
			fwrite(((CTLKItemV1 *)this->Items->Content)->Text, sizeof(char),
				strlen(((CTLKItemV1 *)this->Items->Content)->Text), fOut);
	}

	fclose(fOut);
	return true;*/
}