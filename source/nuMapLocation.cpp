#include "nuMapLocation.h"

#include <fstream>
#include <istream>
#include <iostream>
#include <string>

using namespace Nultima;

void MapLocation::serialize(std::ofstream* stream)
{
    stream->write((char*)&m_cellIdx, 4);
    stream->write((char*)&m_blockX, 4);
    stream->write((char*)&m_blockY, 4);
    stream->write((char*)&m_layer, 4);
}

void MapLocation::deserialize(std::ifstream* stream)
{
    stream->read((char*)&m_cellIdx, 4);
    stream->read((char*)&m_blockX, 4);
    stream->read((char*)&m_blockY, 4);
    stream->read((char*)&m_layer, 4);
}

MapLocation::MapLocation() :
    m_cellIdx(0),
    m_blockX(0),
    m_blockY(0),
    m_layer(0)
{}

MapLocation::MapLocation(unsigned int cellIdx, int blockX, int blockY, int layer) :
    m_cellIdx(cellIdx),
    m_blockX(blockX),
    m_blockY(blockY),
    m_layer(layer)
{}

void MapLocation::getCoordinate(Vec2i& coord)
{
    int cellX = 0xffff & (m_cellIdx>>16);
    int x = cellX * NU_CELL_WIDTH + m_blockX;

    int cellY = 0xffff & m_cellIdx;
    int y = cellY * NU_CELL_WIDTH + m_blockY;

    coord.m_x = x;
    coord.m_y = y;
}

void MapLocation::move(int dx, int dy) 
{
    // TODO [muumi] - asserts

    m_blockX += dx;
    m_blockY += dy;
}
