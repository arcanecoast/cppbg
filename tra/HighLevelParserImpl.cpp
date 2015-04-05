#include <fstream>

#include "HighLevelParserImpl.h"
#include "LogicError.h"

using namespace cppbg_tra;

using std::pair;
using std::ofstream;
using std::ios_base;
using std::endl;
using boost::shared_ptr;

void HighLevelParserImpl::CheckStack(LowLevelParser::container_type::iterator newElement, vector<LowLevelParser::container_type::iterator>& stack)
{
    if (stack.size() == 1 && newElement->GetType() != LowLevelEntry::ENTITY_TEXT) {
        throw LogicError(LogicError::ERROR_NO_TEXT_SECTION_BEFORE_SOUND_SPECIFICATION,
            newElement->GetPosition()
        );
    }

    if (stack.size() == 3) {
        if (newElement->GetType() == LowLevelEntry::ENTITY_TEXT && stack[2]->GetType() == LowLevelEntry::ENTITY_TEXT) {
            throw LogicError(LogicError::ERROR_THIRD_TEXT_SECTION,
                newElement->GetPosition()
            );
        }

        if (newElement->GetType() == LowLevelEntry::ENTITY_SOUND && stack[2]->GetType() == LowLevelEntry::ENTITY_SOUND) {
            throw LogicError(LogicError::ERROR_NO_TEXT_SECTION_BEFORE_SOUND_SPECIFICATION,
                newElement->GetPosition()
            );
        }
    }

    if (stack.size() == 4) {
        if (newElement->GetType() == LowLevelEntry::ENTITY_TEXT) {
            throw LogicError(LogicError::ERROR_THIRD_TEXT_SECTION,
                newElement->GetPosition()
            );
        }

        if (stack[3]->GetType() == LowLevelEntry::ENTITY_SOUND) {
            throw LogicError(LogicError::ERROR_THIRD_SOUND_SPECIFICATION,
                newElement->GetPosition()
            );
        }
    }
}

int HighLevelParserImpl::GetIntID(LowLevelParser::container_type::iterator element)
{
    char* errptr;

    int result = strtol(element->GetContent().c_str(), &errptr, 10);
    if (*errptr != 0) {
        throw LogicError(LogicError::ERROR_WRONG_INDEX_FORMAT,
            element->GetPosition()
        );
    }

    return result;
}

void HighLevelParserImpl::AggregateEntry(int EntryID, vector<LowLevelParser::container_type::iterator>& stack)
{
    switch (stack.size()) {
    case 2: //I T > I
        m_entries.insert(pair<int, shared_ptr<TranslationEntry> >(EntryID, shared_ptr<TranslationEntry>(new TranslationEntry(EntryID, stack[1]->GetContent()))));
        break;
    case 3:
        if (stack[2]->GetType() == LowLevelEntry::ENTITY_SOUND) { //I T S > I
            m_entries.insert(pair<int, shared_ptr<TranslationEntry> >(EntryID, shared_ptr<TranslationEntry>(new TranslationEntry(EntryID, stack[1]->GetContent(), stack[2]->GetContent()))));
        } else { //I T T > I
            m_entries.insert(pair<int, shared_ptr<TranslationEntry> >(EntryID, shared_ptr<TranslationEntry>(new TranslationEntry(EntryID, stack[1]->GetContent(), "", stack[2]->GetContent()))));
        }
        break;
    case 4:
        if (stack[3]->GetType() == LowLevelEntry::ENTITY_SOUND) { //I T T S > I
            m_entries.insert(pair<int, shared_ptr<TranslationEntry> >(EntryID, shared_ptr<TranslationEntry>(new TranslationEntry(EntryID, stack[1]->GetContent(), "", stack[2]->GetContent(), stack[3]->GetContent()))));
        } else { //I T S T > I
            m_entries.insert(pair<int, shared_ptr<TranslationEntry> >(EntryID, shared_ptr<TranslationEntry>(new TranslationEntry(EntryID, stack[1]->GetContent(), stack[2]->GetContent(), stack[3]->GetContent(), ""))));
        }
        break;
    case 5: //I T S T S > I
        m_entries.insert(pair<int, shared_ptr<TranslationEntry> >(EntryID, shared_ptr<TranslationEntry>(new TranslationEntry(EntryID, stack[1]->GetContent(), stack[2]->GetContent(), stack[3]->GetContent(), stack[4]->GetContent()))));
        break;
    }
}

void HighLevelParserImpl::LoadFromFile(const char* Filename)
{
    m_entries.clear();

    LowLevelParser baseParser;
    baseParser.LoadFromFile(Filename);

    vector<LowLevelParser::container_type::iterator> stack; // Stack of values for single entry (@1 = ~sdf~ OR @2 = @3 OR something else ...)
    list<LinkToEntry> assignedValues; // List of referencing entries (like @1 = @2)

    // First lets convert all low-level pieces of entries to high-level entries.

    for (LowLevelParser::container_type::iterator i = baseParser.GetItems().begin(); i != baseParser.GetItems().end(); ++i) {
        if (i->GetType() == LowLevelEntry::ENTITY_MISC) {
            continue;
        }

        if (i->GetType() == LowLevelEntry::ENTITY_ID) {
            if (stack.size() > 0) {
                int idEntry = GetIntID(stack[0]);

                if (m_entries.find(idEntry) != m_entries.end()) {
                    throw LogicError(
                        LogicError::ERROR_REDEFINITION_OF_ENTRY,
                        i->GetPosition()
                    );
                }

                if (stack.size() == 1) {
                    int idAssignTo = GetIntID(i);

                    assignedValues.push_back(LinkToEntry(idEntry, idAssignTo, stack[0]));
                    stack.clear();

                    continue;
                }

                AggregateEntry(idEntry, stack);
                stack.clear();
            }
        } else if (stack.size() == 0 || stack.size() == 5) {
            throw LogicError(
                LogicError::ERROR_FOUND_CONTENT_NOT_ASSIGNED_TO_ENTRY,
                i->GetPosition()
            );
        }

        CheckStack(i, stack);

        stack.push_back(i);
    }

    if (stack.size() > 0) {
        if (stack.size() == 1) {
            throw LogicError(LogicError::ERROR_FOUND_CONTENT_NOT_ASSIGNED_TO_ENTRY,
                stack[0]->GetPosition()
            );
        } else {
            AggregateEntry(GetIntID(stack[0]), stack);
        }

        stack.clear();
    }

    // Finally lets build links between referencing entries (like @1 = @2).

    bool assignedValuesChanged = false;
    while (!assignedValues.empty()) {
        for (list<LinkToEntry>::iterator i = assignedValues.begin(); i != assignedValues.end();) {
            if (m_entries.find(i->id) != m_entries.end()) {
                throw LogicError(
                    LogicError::ERROR_REDEFINITION_OF_ENTRY,
                    i->referenceToAssignee->GetPosition()
                );
            }

            container_type::iterator ref = m_entries.find(i->idAssignedTo);
            if (ref != m_entries.end()) {
                try {
                    pair<int, shared_ptr<TranslationEntry> > reference(
                        i->id, shared_ptr<TranslationEntry>(
                            new TranslationEntry(i->id, ref->second)
                        )
                    );

                    m_entries.insert(reference);
                    assignedValuesChanged = true;
                    assignedValues.erase(i++);
                } catch (const LogicError& error) {
                    if (error.GetType() == LogicError::ERROR_CYCLED_LINKS_FOUND) {
                        throw LogicError(error.GetType(), i->referenceToAssignee->GetPosition());
                    } else {
                        throw;
                    }
                }
            } else {
                ++i;
            }
        }

        if (assignedValuesChanged == false) {
            throw LogicError(LogicError::ERROR_LINK_POINTS_TO_NOWHERE,
                assignedValues.begin()->referenceToAssignee->GetPosition()
            );
        } else {
            assignedValuesChanged = false;
        }
    }
}

void HighLevelParserImpl::SaveToFile(const char* Filename) const
{
    ofstream out;
    out.open(Filename, ios_base::out | ios_base::trunc);
    if (!out.good()) {
        throw ios_base::failure("Unable to open file for writing!");
    }

    for (container_type::const_iterator i = m_entries.begin(); i != m_entries.end(); ++i) {
        out << i->second->GetWritableData() << endl;
    }
}