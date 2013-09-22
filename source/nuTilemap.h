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
    // TODO [muumi] accessors should probably return -1/NULL/something to denote missing tile?
    unsigned int    getTexture  (int id)         { return m_tiles[id].textureId; }
    unsigned int    getTexture  (std::string id) { return m_tiles[m_tileNameIdMap[id]].textureId; }
    Vec3ui          getTileColor(int id)         { return m_tiles[id].color; }
    Vec3ui          getTileColor(std::string id) { return m_tiles[m_tileNameIdMap[id]].color; }
    bool            isTileSolid (int id)         { return m_tiles[id].isSolid; }    
    bool            isTileSolid (std::string id) { return m_tiles[m_tileNameIdMap[id]].isSolid; }    
    int             getNumTiles ()               { return m_tiles.size(); }

private:
    typedef struct
    {
        unsigned int    textureId;
        bool            isSolid;
        Vec3ui          color;
    } BlockInfo;

    int                         m_tileSize;
    std::vector<BlockInfo>      m_tiles;
    std::map<std::string, int>  m_tileNameIdMap;
};

}; // namespace
