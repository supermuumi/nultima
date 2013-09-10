#pragma once

#include "nuMapLocation.h"
#include "nuVec3.h"
#include "nuVec2.h"

#include <string>
#include <vector>
#include <fstream>

#if defined(__APPLE__) || defined(MACOSX)
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

namespace Nultima
{

class Cell;
class Block;

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

    enum
    {
         VERSION_INITIAL = 0,
         VERSION_UNIFIED_COORDS,
         VERSION_BLOCK_REPRESENTATION // each block = 2 bytes on disk now
    } WorldVersion;


    World(std::string fileName);
    ~World() {}

    MapLocation getPlayerStart  () { return m_playerStart; }
    std::tr1::unordered_map<unsigned int, Cell*> getCells   () { return m_cellMap; }

    Cell*       getCellAt       (Vec3i);
    Block*      getBlockAt      (Vec3i);
    void        insertBlock     (Block* block);
    void        clearBlock      (MapLocation location);

    void        serialize       (std::ofstream* stream);
    void        deserialize     (std::ifstream* stream);

    Vec2i       getMinCoordinate();
    Vec2i       getMaxCoordinate();

private:
    std::tr1::unordered_map<unsigned int, Cell*> m_cellMap;
    MapLocation                             m_playerStart;
    char                                    m_version;
};

}; // namespace
