#include "nuTexManager.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuTilemap.h"

using namespace Nultima;

TexManager::TexManager()
{
    // Fill the filename map
    m_textureFilenames["grass"] = "../../assets/textures/grass.jpg";
    m_textureFilenames["water"] = "../../assets/textures/water.gif";
    m_textureFilenames["rock"] = "../../assets/textures/rock.jpg";

    m_tilemap = new Tilemap();
    // TODO should not be in constructor
    m_tilemap->load("../../config/tilemap.json");
}

TexManager::~TexManager()
{
    delete m_tilemap;
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

void TexManager::useTilemap()
{
    Graphics* g = Context::get()->getGraphics();
    g->bindTexture(m_tilemap->getTilemapId());
}

Tilemap::TilemapTexture TexManager::getTextureFromTilemap(std::string id)
{
    return m_tilemap->getTexture(id);
}
