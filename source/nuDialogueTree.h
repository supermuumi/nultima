#pragma once

#include <string>
#include <map>
#include <vector>

namespace Nultima
{
    
class DialogueTree
{
public:
    DialogueTree();
    void load(std::string name);

private:
    std::vector<std::string> m_lines;
    std::map<std::string, int> m_keywords;
};


};
