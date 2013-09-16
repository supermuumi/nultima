#include "nuMapLocation.h"
#include "nuWorld.h"
#include "nuDefs.h"

#include <fstream>
#include <istream>
#include <iostream>
#include <string>

using namespace Nultima;

void MapLocation::serialize(std::ofstream* stream)
{
    m_position.serialize(stream);
}

void MapLocation::deserialize(std::ifstream* stream, int version)
{
    if (version == World::VERSION_INITIAL)
    {
        int idx;
        stream->read((char*)&idx, 4);
        stream->read((char*)&m_position.m_x, 4);
        stream->read((char*)&m_position.m_y, 4);
        stream->read((char*)&m_position.m_z, 4);
    }
    else
    {
        m_position.deserialize(stream);
    }
}

MapLocation::MapLocation() :
    m_position(Vec3i(0, 0, 0))
{}

MapLocation::MapLocation(Vec3i position) :
    m_position(position)
{}

void MapLocation::set(Vec3i vec) 
{
    NU_ASSERT(m_position.m_z >= 0);
    m_position = vec;
}
