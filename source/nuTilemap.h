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

    void           load(std::string fname);
    unsigned int   getTexture(int id) { return m_tiles[id]; }
    Vec3           getTileColor(int id) { return m_tileColors[id]; }
    int            getNumTiles() { return m_tiles.size(); }

private:
    int                       m_tileSize;

    std::vector<unsigned int> m_tiles;
    std::vector<Vec3>         m_tileColors;
};

}; // namespace
