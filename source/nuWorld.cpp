#include "nuDefs.h"
#include "nuWorld.h"
#include "nuMapLocation.h"
#include "nuBlock.h"
#include "nuCell.h"
#include "nuVec3.h"

#include <fstream>

#define STBI_HEADER_FILE_ONLY
#include "stb/stb_image.c"

using namespace Nultima;

// TODO [sampo] streaming: unique tags for each block in the data 

World::World(std::string fileName)
{
    // TODO [sampo] use fileUtils
    std::ifstream worldFile(fileName.c_str(), std::ios_base::binary);
    if (worldFile.is_open())
    {
        deserialize(&worldFile);
        worldFile.close();
    }
}

void World::generateFromPNG(std::string fname)
{
    int w, h, bpp;
    unsigned char* data = stbi_load(fname.c_str(), &w, &h, &bpp, 0);

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {

            int ofs = (y*w + x)*bpp;
            int p = 0;
            for (int b = 0; b < bpp; b++)
                p = (p << 8) | data[ofs+b];

            Vec3i loc(x, y, 0);

            if (p == 0x0000ff) 
                insertBlock(new Block(1, loc));
            if (p == 0x00ff00)
                insertBlock(new Block(2, loc));
            if (p == 0x663300)
                insertBlock(new Block(3, loc));
        }
    }

    free(data);
}

void World::serialize(std::ofstream* stream)
{
    char cWrite = WORLD_TAG_VERSION;
    stream->write(&cWrite, 1);
    cWrite = VERSION_BLOCK_REPRESENTATION;
    stream->write(&cWrite, 1);

    cWrite = WORLD_TAG_PLAYERSTART;
    stream->write(&cWrite, 1);
    MapLocation playerStart(Vec3i(1, 3, 1));
    playerStart.serialize(stream);

    for (std::tr1::unordered_map<unsigned int, Cell*>::iterator it = m_cellMap.begin(); it != m_cellMap.end(); ++it)
    {
        Cell* cell = it->second;
        if (cell)
            cell->serialize(stream);
    }

    cWrite = WORLD_TAG_END;
    stream->write(&cWrite, 1);
}

void World::deserialize(std::ifstream* stream)
{
    bool finished = false;
    while (!finished)
    {
        char tag = -1;
        stream->read(&tag, 1);
        switch (tag)
        {
        case WORLD_TAG_VERSION:
            {
                stream->read(&m_version, 1);
                printf("World file format version: %d\n", m_version);
                break;
            }

        case WORLD_TAG_PLAYERSTART:
            {
                m_playerStart.deserialize(stream, m_version);
                break;
            }

        case WORLD_TAG_CELL:
            {
                Cell* cell = new Cell();
                cell->deserialize(stream, m_version);
                m_cellMap[cell->getIndex()] = cell;
                break;
            }

        case WORLD_TAG_END:
            finished = true;
            break;

        default:
            NU_ASSERT(!"Unkown World serialization tag");
        }
    }
}

void World::insertBlock(Block* block)
{
    Vec3i blockLocation = block->getLocation();
    unsigned int cellIdx = Cell::indexAtLocation(blockLocation);
    
    // Create new Cell if needed
    if (!m_cellMap[cellIdx])
    {
        Vec2i cellCoords(blockLocation.m_x, blockLocation.m_y);
        m_cellMap[cellIdx] = new Cell(cellCoords);
    }

    m_cellMap[cellIdx]->insertBlock(block);
}

void World::clearBlock(MapLocation location)
{
    unsigned int cellIdx = Cell::indexAtLocation(Vec3i(location.m_position.m_x, location.m_position.m_y, location.m_position.m_z));
    if (m_cellMap[cellIdx])
        m_cellMap[cellIdx]->clearBlock(location.m_position);
}

Cell* World::getCellAt(Vec3i v)
{
    unsigned int idx = Cell::indexAtLocation(v);
    return m_cellMap[idx];
}

Block* World::getBlockAt(Vec3i v)
{
    unsigned int idx = Cell::indexAtLocation(v);
    Cell* cell = m_cellMap[idx];
    return (cell) ? (Block*)cell->getBlock(v) : NULL;
}

Vec2i World::getMinCoordinate()
{
    Vec2i min(0,0);
    for (std::tr1::unordered_map<unsigned int, Cell*>::iterator it = m_cellMap.begin(); it != m_cellMap.end(); ++it)
    {
        Cell* cell = it->second;
        if (cell)
        {
            Vec2i position = cell->getPosition();
            if (position.m_x < min.m_x)
                min.m_x = position.m_x;
            if (position.m_y < min.m_y)
                min.m_y = position.m_y;
        }
    }
    return min;
}

Vec2i World::getMaxCoordinate()
{
    Vec2i max(0,0);
    for (std::tr1::unordered_map<unsigned int, Cell*>::iterator it = m_cellMap.begin(); it != m_cellMap.end(); ++it)
    {
        Cell* cell = it->second;
        if (cell)
        {
            Vec2i position = cell->getPosition();
            if (position.m_x > max.m_x)
                max.m_x = position.m_x;
            if (position.m_y > max.m_y)
                max.m_y = position.m_y;
        }
    }
    return max;
}
