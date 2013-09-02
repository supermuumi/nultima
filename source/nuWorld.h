#pragma once

#include "nuMapLocation.h"
#include "nuCell.h"

#include <string>
#include <unordered_map>

namespace Nultima
{

class World
{
public:

    typedef enum
    {
        WORLD_TAG_VERSION = 0,
        WORLD_TAG_PLAYERSTART,
        WORLD_TAG_CELL,
        WORLD_TAG_BLOCK,
        WORLD_TAG_END
    } SerializationTags;

    World(std::string fileName);
    ~World() {}

    MapLocation         getPlayerStart  ()  { return m_playerStart; }
    std::vector<Cell*>  getCells        ()  { return m_loadedCells; }

private:
    std::unordered_map<unsigned int, Cell*> m_cellMap;
    std::vector<Cell*>                      m_loadedCells;
    MapLocation                             m_playerStart;
};

}; // namespace
