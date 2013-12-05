#ifndef CPPBG_TRA_TRANSLATIONENTRY 
#define CPPBG_TRA_TRANSLATIONENTRY

/**
 * Author: metamaker
 * Creation date: 22.08.2013 14:05
 */

#include <string>
#include <boost/shared_ptr.hpp>

using std::string;
using boost::shared_ptr;

namespace cppbg_tra
{
	/**
	 * This class describes entry from WeiDU .tra file.
	 */
	class TranslationEntry
	{
	public:
		explicit TranslationEntry(int ID, const string& MainText, const string& MainSound = "",
			const string& SecondaryText = "", const string& SecondarySound = "");
		explicit TranslationEntry(int ID, shared_ptr<const TranslationEntry> LinkedEntry);

		bool IsAttachedToOtherEntry() const { return m_linkedEntry.get() != 0; }
		shared_ptr<const TranslationEntry> GetAttachedEntry() const { return m_linkedEntry; }

		inline string GetMainText() const;
		inline string GetMainSound() const;
		inline string GetSecondaryText() const;
		inline string GetSecondarySound() const;
		int GetId() const { return m_id; }

        string GetWritableData() const;

		void AttachToOtherEntry(shared_ptr<const TranslationEntry> LinkedEntry);
		void Detach();

		inline void SetMainText(const string& Text);
		inline void SetMainSound(const string& Sound);
		inline void SetSecondaryText(const string& Text);
		inline void SetSecondarySound(const string& Sound);
		void SetId(int ID) { m_id = ID; }

	private:
		shared_ptr<const TranslationEntry> m_linkedEntry;

		string m_mainText;
		string m_mainSound;

		string m_secondaryText;
		string m_secondarySound;

		int m_id;
	};
};

#endif // CPPBG_TRA_TRANSLATIONENTRY