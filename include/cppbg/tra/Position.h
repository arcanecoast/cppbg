#ifndef CPPBG_TRA_POSITION 
#define CPPBG_TRA_POSITION

/**
 * Author: metamaker
 * Creation date: 22.08.2013 14:05
 */

namespace cppbg_tra
{
    class Position
    {
    public:
        Position() :
          m_indexOfLine(-1),
          m_indexOfCharacterInLine(-1),
          m_physicalOffset(-1)
        {
        }

        explicit Position(int IndexOfLine, int IndexOfCharacterInLine, int PhysicalOffset) :
            m_indexOfLine(IndexOfLine),
            m_indexOfCharacterInLine(IndexOfCharacterInLine),
            m_physicalOffset(PhysicalOffset)
        {
        }

        int GetIndexOfLine() const { return m_indexOfLine; }
        int GetPlaceInLine() const { return m_indexOfCharacterInLine; }
        int GetPhysicalOffset() const { return m_physicalOffset; }

        void SetIndexOfLine(int IndexOfLine) { m_indexOfLine = IndexOfLine; }
        void SetPlaceInLine(int IndexOfCharacterInLine) { m_indexOfCharacterInLine = IndexOfCharacterInLine; }
        void SetPhysicalOffset(int PhysicalOffset) { m_physicalOffset = PhysicalOffset; }

        void Update(int IndexOfLine, int IndexOfCharacterInLine, int PhysicalOffset)
        {
            SetIndexOfLine(IndexOfLine);
            SetPlaceInLine(IndexOfCharacterInLine);
            SetPhysicalOffset(PhysicalOffset);
        }

    private:
        int m_indexOfLine;
        int m_indexOfCharacterInLine;
        int m_physicalOffset;
    };
};

#endif // CPPBG_TRA_POSITION