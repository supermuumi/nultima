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

    void            loadTilemap         (char* fname);
    unsigned int    getTexture          (std::string name);
    unsigned int    getTilemapTexture   (int id);
	int             getNumTiles         () { return m_tilemap->getNumTiles(); }
    Tilemap*        getTilemap          () { return m_tilemap; }

private:
    unsigned int loadTexture(std::string name);

    std::map<std::string, unsigned int> m_textures;
    std::map<std::string, std::string>  m_textureFilenames;

    Tilemap* m_tilemap;
};

};
