#include "nuFileUtils.h"
#include "nuDialogueTree.h"

#include "rapidjson/document.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace Nultima;

DialogueTree::DialogueTree()
{
}

void DialogueTree::load(std::string fname)
{
    rapidjson::Document doc = FileUtils::readJSON(fname);

    // assuming valid json
    assert(!doc.HasParseError());

    // parse dialogue lines
    const rapidjson::Value& lines = doc["lines"];
    for (rapidjson::SizeType i = 0; i < lines.Size(); i++)
        m_lines.push_back(lines[i].GetString());

    const rapidjson::Value& keywords = doc["keywords"];
    for (rapidjson::Value::ConstMemberIterator itr = keywords.MemberBegin(); itr != keywords.MemberEnd(); ++itr)
    {
        std::string s = itr->name.GetString();
        int idx = itr->value.GetInt();
        m_keywords[s] = idx;
    }
}


