#pragma once

#include <string>
#include <vector>
#include <map>
#include "nuVec3.h"

namespace Nultima
{

class Tilemap
{
public:
    Tilemap();
    ~Tilemap();

    void            load        (std::string fname);
    unsigned int    getTexture  (int id)    { return m_tiles[id].textureId; }
    Vec3ui          getTileColor(int id)    { return m_tiles[id].color; }
    char            getNumTiles ()          { return (char)m_tiles.size(); }
    bool            isTileSolid (int id)    { return m_tiles[id].isSolid; }    

private:
    typedef struct
    {
        unsigned int    textureId;
        bool            isSolid;
        Vec3ui          color;
    } BlockInfo;

    int                         m_tileSize;
    std::vector<BlockInfo>      m_tiles;
};

}; // namespace
