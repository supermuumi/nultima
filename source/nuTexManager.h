#pragma once

#include <string>
#include <map>

namespace Nultima
{

class TexManager
{
public:
    TexManager();
    ~TexManager() {}

    unsigned int getTexture(std::string name);

private:
    unsigned int loadTexture(std::string name);

    std::map<std::string, unsigned int> m_textures;
    std::map<std::string, std::string>  m_textureFilenames;
};

};
