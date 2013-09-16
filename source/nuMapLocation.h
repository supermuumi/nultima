#pragma once

#include "nuDefs.h"
#include "nuVec3.h"

#include <fstream>

namespace Nultima
{

class MapLocation
{
public:
    MapLocation();
    MapLocation(Vec3i position);
    ~MapLocation() {};

    void    serialize       (std::ofstream* stream);
    void    deserialize     (std::ifstream* stream, int version);

    void    set             (Vec3i d);

    Vec3i   m_position;
};

};
