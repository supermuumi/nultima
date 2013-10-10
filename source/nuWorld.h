#pragma once

#include "nuDefs.h"
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
         VERSION_BLOCK_REPRESENTATION,   // each block = 2 bytes on disk now
         VERSION_BLOCK_BLOCKTYPE_NUINT32 // muumi 9-oct-2013: change block type to uint 
    } WorldVersion;

    World() {}
    World(std::string fileName);
    ~World() {}

    void generateFromPNG(std::string fname);

    Vec3i       getPlayerStart  () { return m_playerStart; }
    std::tr1::unordered_map<unsigned int, Cell*> getCells   () { return m_cellMap; }

    Cell*       getCellAt       (Vec3i);
    Block*      getBlockAt      (Vec3i);
    void        insertBlock     (Block* block);
    void        clearBlock      (Vec3i location);

    void        serialize       (std::ofstream* stream);
    void        deserialize     (std::ifstream* stream);

    Vec2i       getMinCoordinate();
    Vec2i       getMaxCoordinate();

    void        setPlayerStart(Vec3i v) { m_playerStart = v; }

private:
    int         getMapPixel(unsigned char* data, int x, int y, int w);
    std::tr1::unordered_map<unsigned int, Cell*> m_cellMap;
    Vec3i       m_playerStart;
    NuUInt32    m_version;
};

}; // namespace
