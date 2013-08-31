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

    stream >> cellIdx;
    stream >> blockX;
    stream >> blockY;

    location.setCellIdx(cellIdx);
    location.setBlockX(blockX);
    location.setBlockY(blockY);

    return stream;
}
