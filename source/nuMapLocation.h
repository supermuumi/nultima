#pragma once

#include "nuDefs.h"
#include <fstream>

namespace Nultima
{

class MapLocation
{
public:
    MapLocation() {};
    ~MapLocation() {};

    unsigned int getCellIdx () { return m_cellIdx; }
    int getBlockX           () { return m_blockX; }
    int getBlockY           () { return m_blockY; }
    int getLayer            () { return m_layer; }

    void    setCellIdx  (unsigned int cellIdx)  { m_cellIdx = cellIdx; }
    void    setBlockX   (int blockX)            { m_blockX = blockX; }
    void    setBlockY   (int blockY)            { m_blockY = blockY; }
    void    setLayer    (int layer)             { m_layer = layer; }

    int     getCellX    ();
    int     getCellY    ();

private:
    friend std::ifstream& operator>> (std::ifstream& stream, MapLocation& location);

    unsigned int    m_cellIdx;
    int             m_blockX;
    int             m_blockY;
    int             m_layer;
};

};