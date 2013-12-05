#include "HighLevelParserImpl.h"
#include "HighLevelParser.h"

using namespace cppbg_tra;

HighLevelParser::HighLevelParser() : pimpl(new HighLevelParserImpl()) {}

void HighLevelParser::LoadFromFile(const char* Filename) { pimpl->LoadFromFile(Filename); }
void HighLevelParser::SaveToFile(const char* Filename) const { pimpl->SaveToFile(Filename); }

const HighLevelParser::container_type& HighLevelParser::GetItems() const { return pimpl->GetItems(); }
HighLevelParser::container_type& HighLevelParser::GetItems() { return pimpl->GetItems(); }