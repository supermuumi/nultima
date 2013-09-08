#pragma once

#include <string>
#include <map>
#include "nuDefs.h"
#include "nuTilemap.h"

namespace Nultima
{

class TexManager
{

    struct TextureInfo;

public:
    TexManager();
    ~TexManager();

    unsigned int getTexture(std::string name);
    void         useTilemap();
    Tilemap::TilemapTexture  getTilemapTexture(std::string id);

private:
    unsigned int loadTexture(std::string name);

    std::map<std::string, unsigned int> m_textures;
    std::map<std::string, std::string>  m_textureFilenames;

    Tilemap* m_tilemap;
};

};
