#pragma once

#include "nuDefs.h"
#include "nuVec2.h"

#include <fstream>

namespace Nultima
{

class MapLocation
{
public:
    MapLocation();
    MapLocation(unsigned int cellIdx, int blockX, int blockY, int layer);
    ~MapLocation() {};

    void    getCoordinate   (Vec2i& coord);
    void    serialize       (std::ofstream* stream);
    void    deserialize     (std::ifstream* stream);

private:

    Vec2i           m_coordinate;

    unsigned int    m_cellIdx;
    int             m_blockX;
    int             m_blockY;
    int             m_layer;
};

};