#include "nuDefs.h"
#include "nuWorld.h"
#include "nuMapLocation.h"
#include "nuBlock.h"
#include "nuCell.h"
#include "nuVec3.h"

#include <fstream>

#define WORLD_VERSION_INITIAL 0

using namespace Nultima;

// TODO [sampo] streaming: unique tags for each block in the data 

World::World(std::string fileName)
{
    // TODO [sampo] use fileUtils
    std::ifstream worldFile(fileName.c_str(), std::ios_base::binary);
    deserialize(&worldFile);
    worldFile.close();
}

void World::serialize(std::ofstream* stream)
{
    char cWrite = WORLD_TAG_VERSION;
    stream->write(&cWrite, 1);
    cWrite = WORLD_VERSION_INITIAL;
    stream->write(&cWrite, 1);

    cWrite = WORLD_TAG_PLAYERSTART;
    stream->write(&cWrite, 1);
    MapLocation playerStart(0, 1, 3, 0);
    playerStart.serialize(stream);

    for (std::vector<Cell*>::iterator it = m_loadedCells.begin(); it != m_loadedCells.end(); ++it)
        (*it)->serialize(stream);

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
                char version = -1;
                stream->read(&version, 1);
                printf("World file format version: %d\n", version);
                break;
            }

        case WORLD_TAG_PLAYERSTART:
            {
                m_playerStart.deserialize(stream);
                break;
            }

        case WORLD_TAG_CELL:
            {
                Cell* cell = new Cell();
                cell->deserialize(stream);
                m_loadedCells.push_back(cell);
                // TODO [sampo] m_cellMap and streaming
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

void World::insertBlock(MapLocation location, char block)
{
    // TODO
}

void World::removeBlock(MapLocation location)
{
    // TODO
}

Cell* World::getCellAt(Vec3ui v)
{
    for (std::vector<Cell*>::iterator it = m_loadedCells.begin(); it != m_loadedCells.end(); ++it)
    {
        Cell* cell = *it;
        Vec2i cellPos = cell->getPosition();
        if (v.m_x >= cellPos.m_x && v.m_x < cellPos.m_x+NU_CELL_WIDTH &&
            v.m_y >= cellPos.m_y && v.m_y < cellPos.m_y+NU_CELL_HEIGHT)
            return cell;
    }

    return NULL;
}
