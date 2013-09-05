#include "nuTexManager.h"
#include "nuContext.h"
#include "nuGraphics.h"

using namespace Nultima;

TexManager::TexManager()
{
    // Fill the filename map
    m_textureFilenames["grass"] = "../../assets/textures/grass.jpg";
    m_textureFilenames["water"] = "../../assets/textures/water.gif";
}

unsigned int TexManager::loadTexture(std::string name)
{
    Graphics* graphics = Context::get()->getGraphics();
    m_textures[name] = graphics->loadTexture(m_textureFilenames[name]);
    return m_textures[name];
}

unsigned int TexManager::getTexture(std::string name)
{
    std::map<std::string,unsigned int>::const_iterator tit = m_textures.find(name.c_str());
    if (tit != m_textures.end())
	    return tit->second;

    return loadTexture(name);
}
