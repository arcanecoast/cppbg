#include <fstream>
#include <cstring>
#include <stdexcept>

#include <cppbg/tlk_v1/TalkTableFile.h>

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
    ofstream out(Filename, ios_base::out | ios::binary);
    if (!out.good()) {
        throw ios_base::failure("Unable to open file for writing!");
    }

    out.write(TalkTable::SIGNATURE, 8);

    unsigned long entriesCount = this->GetItems().size();
    long contentOffset = entriesCount * 26 + 18; /* 26 bytes per header talktable entry + 18 bytes per header info*/

    out.write(reinterpret_cast<const char*>(&m_language), 2);
    out.write(reinterpret_cast<const char*>(&entriesCount), 4);
    out.write(reinterpret_cast<const char*>(&contentOffset), 4);

    char soundResRef[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    long offsetToEntryContent = 0;

    for (unsigned long i = 0; i < entriesCount; i++)
    {
        short flags = GetItems()[i].GetFlags();
        strncpy(soundResRef, GetItems()[i].GetSoundResRef().c_str(), 8);
        long stringSize = GetItems()[i].GetText().length();
        long volumeVariance = GetItems()[i].GetVolumeVariance();
        long pitchVariance = GetItems()[i].GetPitchVariance();

        out.write(reinterpret_cast<const char*>(&flags), 2);
        out.write(soundResRef, 8);
        out.write(reinterpret_cast<const char*>(&volumeVariance), 4);
        out.write(reinterpret_cast<const char*>(&pitchVariance), 4);

        if (stringSize > 0) {
            out.write(reinterpret_cast<const char*>(&offsetToEntryContent), 4);
            out.write(reinterpret_cast<const char*>(&stringSize), 4);
        } else {
            long nullValue = 0;

            out.write(reinterpret_cast<const char*>(&nullValue), 4);
            out.write(reinterpret_cast<const char*>(&nullValue), 4);
        }

        offsetToEntryContent += stringSize;
    }

    for (unsigned long i = 0; i < entriesCount; i++)
    {
        if (!GetItems()[i].GetText().empty()) {
            out.write(GetItems()[i].GetText().c_str(), GetItems()[i].GetText().length());
        }
    }
}