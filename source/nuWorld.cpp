#include "nuWorld.h"
#include "nuMapLocation.h"
#include "nuBlock.h"

#include <fstream>

#define WORLD_VERSION_INITIAL 0

using namespace Nultima;

// TODO [sampo] streaming: unique tags for each block in the data 

World::World(std::string fileName)
{
#if 1
    std::ofstream outFile("foo.world", std::ios_base::binary);
    if (outFile.is_open())
    {
        char cWrite = WORLD_TAG_VERSION;
        outFile.write(&cWrite, 1);
        cWrite = WORLD_VERSION_INITIAL;
        outFile.write(&cWrite, 1);

        cWrite = WORLD_TAG_PLAYERSTART;
        outFile.write(&cWrite, 1);
        MapLocation playerStart(0, 1, 3, 0);
        playerStart.serialize(&outFile);

        {
            cWrite = WORLD_TAG_CELL;
            outFile.write(&cWrite, 1);
            Cell cell(0, 0);
            for (int x=0; x<NU_CELL_WIDTH; x++)
            for (int y=0; y<NU_CELL_HEIGHT; y++)
                cell.insertBlock(Block::GRASS, Vec2i(x, y), 0);
            for (int x=0; x<NU_CELL_WIDTH; x+=2)
            for (int y=0; y<NU_CELL_HEIGHT; y+=2)
                cell.insertBlock(Block::ROCK, Vec2i(x, y), 1);
            cell.serialize(&outFile);
        }

        {
            cWrite = WORLD_TAG_CELL;
            outFile.write(&cWrite, 1);
            Cell cell(-1, -1);
            for (int x=0; x<NU_CELL_WIDTH; x++)
            for (int y=0; y<NU_CELL_HEIGHT; y++)
                cell.insertBlock(Block::WATER, Vec2i(x, y), 0);
            cell.serialize(&outFile);
        }

        {
            cWrite = WORLD_TAG_CELL;
            outFile.write(&cWrite, 1);
            Cell cell(-2, -2);
            for (int x=0; x<NU_CELL_WIDTH; x++)
            for (int y=0; y<NU_CELL_HEIGHT; y++)
                cell.insertBlock(Block::GRASS, Vec2i(x, y), 0);
            cell.serialize(&outFile);
        }

        {
            cWrite = WORLD_TAG_CELL;
            outFile.write(&cWrite, 1);
            Cell cell(1, 1);
            for (int x=0; x<NU_CELL_WIDTH; x++)
            for (int y=0; y<NU_CELL_HEIGHT; y++)
                cell.insertBlock(Block::WATER, Vec2i(x, y), 0);
            cell.serialize(&outFile);
        }

        cWrite = WORLD_TAG_END;
        outFile.write(&cWrite, 1);
    }
    outFile.close();
#endif

#if 1
    std::ifstream worldFile(fileName.c_str(), std::ios_base::binary);
    bool finished = false;
    while (!finished)
    {
        char tag = -1;
        worldFile.read(&tag, 1);
        switch (tag)
        {
        case WORLD_TAG_VERSION:
            {
                char version = -1;
                worldFile.read(&version, 1);
                printf("World file format version: %d\n", version);
                break;
            }

        case WORLD_TAG_PLAYERSTART:
            {
                m_playerStart.deserialize(&worldFile);
                break;
            }

        case WORLD_TAG_CELL:
            {
                Cell* cell = new Cell();
                cell->deserialize(&worldFile);
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
    worldFile.close();
#endif
}

