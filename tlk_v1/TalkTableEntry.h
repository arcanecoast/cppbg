#ifndef CPPBG_TLK_V1_TALKTABLEENTRY
#define CPPBG_TLK_V1_TALKTABLEENTRY

#include <string>

using std::string;

namespace cppbg_tlk_v1
{
    class TalkTableEntry
    {
    private:
	    short  m_flags;
	    long   m_volumeVariance;
        long   m_pitchVariance;
        string m_soundResRef;
        string m_text;
        
    public:
        TalkTableEntry(short Flags, long VolumeVariance, long PitchVariance,
            const string& SoundResRef, const string Text) :
            m_flags(Flags),
            m_volumeVariance(VolumeVariance),
            m_pitchVariance(PitchVariance),
            m_text(Text)
        {
            SetSoundResRef(SoundResRef);
        }

        short  GetFlags() const { return m_flags; }
        long   GetPitchVariance() const { return m_pitchVariance; }
        long   GetVolumeVariance() const { return m_volumeVariance; }
        string GetText() const { return m_text; }
        string GetSoundResRef() const { return m_soundResRef; }

        void   SetFlags(short Flags) { m_flags = Flags; }
        void   SetPitchVariance(long Val) { m_pitchVariance = Val; }
        void   SetVolumeVariance(long Val) { m_volumeVariance = Val; }
        void   SetText(const string& Val) { m_text = Val; }
        void   SetSoundResRef(const string& Val)
        {
            if (Val.length() > 8) {
                m_soundResRef = Val.substr(0, 8);
            } else {
                m_soundResRef = Val;
            }
        }
    };
}

#endif //CPPBG_TLK_V1_TALKTABLEENTRY