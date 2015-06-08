#include <cppbg/tra/HighLevelParser.h>
#include <cppbg/tra/WeiDUModTranslation.h>

using namespace cppbg_tra;

WeiDUModTranslation::WeiDUModTranslation() : pimpl(new HighLevelParser()) {}

void WeiDUModTranslation::LoadFromFile(const char* Filename) { pimpl->LoadFromFile(Filename); }
void WeiDUModTranslation::SaveToFile(const char* Filename) const { pimpl->SaveToFile(Filename); }

cppbg_tra::WeiDUModTranslation::const_iterator cppbg_tra::WeiDUModTranslation::Get(int id) const
{
    return pimpl->GetItems().find(id);
}

cppbg_tra::WeiDUModTranslation::iterator cppbg_tra::WeiDUModTranslation::Get(int id)
{
    return pimpl->GetItems().find(id);
}

cppbg_tra::WeiDUModTranslation::const_iterator cppbg_tra::WeiDUModTranslation::operator[](int id) const
{
    return pimpl->GetItems().find(id);
}

cppbg_tra::WeiDUModTranslation::iterator cppbg_tra::WeiDUModTranslation::operator[](int id)
{
    return pimpl->GetItems().find(id);
}

bool cppbg_tra::WeiDUModTranslation::Insert(const TranslationEntry& entry)
{
    std::pair<iterator, bool> result = pimpl->GetItems().insert(std::make_pair(entry.GetId(), shared_ptr<TranslationEntry>(new TranslationEntry(entry))));

    return result.second;
}

size_t cppbg_tra::WeiDUModTranslation::Remove(int id)
{
    return pimpl->GetItems().erase(id);
}

size_t cppbg_tra::WeiDUModTranslation::Size() const
{
    return pimpl->GetItems().size();
}

bool cppbg_tra::WeiDUModTranslation::Empty() const
{
    return pimpl->GetItems().empty();
}

cppbg_tra::WeiDUModTranslation::const_iterator cppbg_tra::WeiDUModTranslation::Begin() const
{
    return pimpl->GetItems().begin();
}

cppbg_tra::WeiDUModTranslation::iterator cppbg_tra::WeiDUModTranslation::Begin()
{
    return pimpl->GetItems().begin();
}

cppbg_tra::WeiDUModTranslation::const_iterator cppbg_tra::WeiDUModTranslation::End() const
{
    return pimpl->GetItems().end();
}

cppbg_tra::WeiDUModTranslation::iterator cppbg_tra::WeiDUModTranslation::End()
{
    return pimpl->GetItems().end();
}

cppbg_tra::WeiDUModTranslation::const_reverse_iterator cppbg_tra::WeiDUModTranslation::Rbegin() const
{
    return pimpl->GetItems().rbegin();
}

cppbg_tra::WeiDUModTranslation::reverse_iterator cppbg_tra::WeiDUModTranslation::Rbegin()
{
    return pimpl->GetItems().rbegin();
}

cppbg_tra::WeiDUModTranslation::const_reverse_iterator cppbg_tra::WeiDUModTranslation::Rend() const
{
    return pimpl->GetItems().rend();
}

cppbg_tra::WeiDUModTranslation::reverse_iterator cppbg_tra::WeiDUModTranslation::Rend()
{
    return pimpl->GetItems().rend();
}
