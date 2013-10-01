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

    // TODO [muumi] sanitize accessors
    int             getTexture  (int id)         { return m_tiles[id].textureId; }
    int             getTexture  (std::string id) { return m_tiles[m_tileNameIdMap[id]].textureId; }
    unsigned int    getTileIndex(std::string id) { return m_tileNameIdMap[id]; }
    Vec3ui          getTileColor(int id)         { return m_tiles[id].color; }
    Vec3ui          getTileColor(std::string id) { return m_tiles[m_tileNameIdMap[id]].color; }
    bool            isTileSolid (int id)         { return m_tiles[id].isSolid; }    
    bool            isTileSolid (std::string id) { return m_tiles[m_tileNameIdMap[id]].isSolid; }    
    std::string     getTextureId(int id)         { return m_tiles[id].name; }
    int             getNumTiles ()               { return m_tiles.size(); }
    int             getNextTile (int n, int d);

private:
    typedef struct
    {
        int         textureId;
        bool        isSolid;
        Vec3ui      color;
        std::string name;
    } BlockInfo;

    int                         m_tileSize;
    std::vector<BlockInfo>      m_tiles;
    std::map<std::string, int>  m_tileNameIdMap;
};

}; // namespace
