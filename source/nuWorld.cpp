#include "nuDefs.h"
#include "nuWorld.h"
#include "nuBlock.h"
#include "nuCell.h"
#include "nuContext.h"
#include "nuTexManager.h"
#include "nuTilemap.h"
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

// TODO assuming bpp=3
// TODO handle out of bounds?
int World::getMapPixel(unsigned char* data, int x, int y, int w)
{
    int ofs = (y*w + x)*3;
    int p = (data[ofs]<<16) | (data[ofs+1] << 8) | data[ofs+2];
    return p;
}

void World::generateFromPNG(std::string fname)
{
    int w, h, bpp;
    unsigned char* data = stbi_load(fname.c_str(), &w, &h, &bpp, 0);

    TexManager* tex = Context::get()->getTexManager();
    Tilemap* tilemap = tex->getTilemap();

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Vec3i loc(x, h-y-1, 0);
            int p = getMapPixel(data, x, y, w);

/*
0x0000ff = water
0x00ff00 = park (using grassland2)
0xffffff = generic ground (using grassland1)
0x00ffff = Unkown (using mountain_small)
0xffff00 = highway / road (using road_vertical)
0xff00ff = Unkown (using forest_normal)
*/
            // water, check for shoreline
            std::string id;

            bool sameLeft  = !(x > 0 && getMapPixel(data, x-1, y, w) != p);
            bool sameRight = !(x < w-1 && getMapPixel(data, x+1, y, w) != p);
            bool sameUp    = !(y > 0 && getMapPixel(data, x, y-1, w) != p);
            bool sameDown  = !(y < h-1 && getMapPixel(data, x, y+1, w) != p);

            if (p == 0x0000ff) {

                id = "sea_shallow";


                // TODO not all cases covered, also need more tiles...
                if (!sameRight)
                {
                    if (!sameDown)    id = "shore_topleft";
                    else if (!sameUp) id = "shore_bottomleft";
                    else             id = "shore_left";
                }
                else if (!sameLeft)
                {
                    if (!sameDown)    id = "shore_topright";
                    else if (!sameUp) id = "shore_bottomright";
                    else             id = "shore_right";
                }
            }
            else if (p == 0x00ff00) id = "grassland";
            else if (p == 0xffffff) id = "plains";
            else if (p == 0x00ffff) id = "mountain_medium";
            // roads
            // TODO [muumi] not all cases
            else if (p == 0xffff00) 
            {
                id = "road_vert";
                if (sameRight && sameLeft && sameUp && sameDown) id = "road_crossroad";
                else if (sameRight && sameLeft && sameDown) id = "road_T";
                else if (sameRight && sameLeft) id = "road_horiz";
                else if (sameRight && sameUp) id = "road_L";
                else if (sameRight && sameDown) id = "road_L90";                
                else if (sameLeft && sameDown) id = "road_L180";
                else if (sameLeft && sameUp) id = "road_L270";
                else if (sameRight && sameLeft) id = "road_horiz";
            }
            else if (p == 0xff00ff) id = "forest_normal"; 
            else
            {
                printf("p = %x\n", p);
                NU_ASSERT(!"unknown block type");
            }
            
            insertBlock(new Block((char)tilemap->getTileIndex(id), loc));
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
    Vec3i playerStart(1, 3, 1);
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
                if (m_version == World::VERSION_INITIAL)
                {
                    int idx;
                    stream->read((char*)&idx, 4);
                    stream->read((char*)&m_playerStart.m_x, 4);
                    stream->read((char*)&m_playerStart.m_y, 4);
                    stream->read((char*)&m_playerStart.m_z, 4);
                }
                else
                {
                    m_playerStart.deserialize(stream);
                }
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

void World::clearBlock(Vec3i location)
{
    unsigned int cellIdx = Cell::indexAtLocation(location);
    if (m_cellMap[cellIdx])
        m_cellMap[cellIdx]->clearBlock(location);
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
