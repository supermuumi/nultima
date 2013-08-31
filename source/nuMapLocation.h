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

    int getCellIdx  () { return m_cellIdx; }
    int getBlockX   () { return m_blockX; }
    int getBlockY   () { return m_blockY; }

    void    setCellIdx  (int cellIdx)   { m_cellIdx = cellIdx; }
    void    setBlockX   (int blockX)    { m_blockX = blockX; }
    void    setBlockY   (int blockY)    { m_blockY = blockY; }

private:
    friend std::ifstream& operator>> (std::ifstream& stream, MapLocation& location);

    int m_cellIdx;
    int m_blockX;
    int m_blockY;
};

};