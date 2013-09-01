#include "nuMapLocation.h"

#include <fstream>
#include <istream>
#include <iostream>
#include <string>

using namespace Nultima;

std::ifstream& Nultima::operator>> (std::ifstream& stream, MapLocation& location)
{
    int cellIdx= 0;
    int blockX= 0;
    int blockY = 0;
    int layer = 0;

    stream >> cellIdx;
    stream >> blockX;
    stream >> blockY;
    stream >> layer;

    location.setCellIdx(cellIdx);
    location.setBlockX(blockX);
    location.setBlockY(blockY);
    location.setLayer(layer);

    return stream;
}

int MapLocation::getCellX()
{
    return 0xffff & (m_cellIdx>>16);
}

int MapLocation::getCellY()
{
    return 0xffff & m_cellIdx;
}
