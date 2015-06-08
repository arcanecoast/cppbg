#include "TranslationEntry.h"
#include "TranslationEntryUtilities.h"
#include "LogicError.h"

using namespace cppbg_tra;

TranslationEntry::TranslationEntry(int ID, const string& MainText, const string& MainSound,
	const string& SecondaryText, const string& SecondarySound) :
	m_linkedEntry(static_cast<const TranslationEntry*>(0)),
	m_mainText(MainText),
	m_mainSound(MainSound),
	m_secondaryText(SecondaryText),
	m_secondarySound(SecondarySound),
	m_id(ID)
{
}

TranslationEntry::TranslationEntry(int ID, shared_ptr<const TranslationEntry> LinkedEntry) :
	m_id(ID)
{
    AttachToOtherEntry(LinkedEntry);
}

string TranslationEntry::GetMainText() const
{
	if (m_linkedEntry.get()) {
		return m_linkedEntry->GetMainText();
	} else {
		return m_mainText;
	}
}

string TranslationEntry::GetMainSound() const
{
	if (m_linkedEntry.get()) {
		return m_linkedEntry->GetMainSound();
	} else {
		return m_mainSound;
	}
}

string TranslationEntry::GetSecondaryText() const 
{
	if (m_linkedEntry.get()) {
		return m_linkedEntry->GetSecondaryText();
	} else {
		return m_secondaryText;
	}
}

string TranslationEntry::GetSecondarySound() const
{
	if (m_linkedEntry.get()) {
		return m_linkedEntry->GetSecondarySound();
	} else {
		return m_secondarySound;
	}
}

string TranslationEntry::GetWritableData() const
{
    char idval[16];
    string result = "@";

    sprintf(idval, "%i", m_id);
    result += idval;
    result += " = ";

    if (m_linkedEntry.get()) {
        result += "@";
        sprintf(idval, "%i", m_linkedEntry->GetId());
        result += idval;
    } else {
        result += EscapeTranslationContent(GetMainText());
        if (!GetMainSound().empty()) {
            result += " [" + GetMainSound() + ']';
        }

        if (!GetSecondaryText().empty()) {
            result += ' ' + EscapeTranslationContent(GetSecondaryText());

            if (!GetSecondarySound().empty()) {
                result += " [" + GetSecondarySound() + ']';
            }
        }
    }

    return result;
}

void TranslationEntry::SetMainText(const string& Text)
{
	Detach();
	m_mainText = Text;
}

void TranslationEntry::SetMainSound(const string& Sound)
{
	Detach();
	m_mainSound = Sound;
}

void TranslationEntry::SetSecondaryText(const string& Text)
{
	Detach();
	m_secondaryText = Text;
}

void TranslationEntry::SetSecondarySound(const string& Sound)
{
	Detach();
	m_secondarySound = Sound;
}

void TranslationEntry::AttachToOtherEntry(shared_ptr<const TranslationEntry> LinkedEntry)
{
	if (LinkedEntry.get() == 0) {
		Detach();
	} else {
        shared_ptr<const TranslationEntry> curptr = LinkedEntry->GetAttachedEntry();

        // Check if there are no cycles in links

        while (curptr.get() != 0) {
            if (curptr->GetId() == this->GetId()) {
                throw LogicError(LogicError::ERROR_CYCLED_LINKS_FOUND);
            }

            curptr = curptr->GetAttachedEntry();
        }

		m_linkedEntry = LinkedEntry;
	}
}

void TranslationEntry::Detach()
{
	if (m_linkedEntry.get()) {
		m_mainText       = m_linkedEntry->GetMainText();
		m_mainSound      = m_linkedEntry->GetMainSound();
		m_secondaryText  = m_linkedEntry->GetSecondaryText();
		m_secondarySound = m_linkedEntry->GetSecondarySound();

		m_linkedEntry.reset(static_cast<const TranslationEntry*>(0));
	}
}