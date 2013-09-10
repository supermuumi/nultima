#include "nuCell.h"
#include "nuBlock.h"
#include "nuWorld.h"
#include "nuDefs.h"
#include "nuContext.h"
#include "nuGraphics.h"

#include <cstring>

using namespace Nultima;

Cell::Cell(Vec2i position)
{
    memset(m_blocks, 0, sizeof(Block*)*NU_MAX_LAYERS*NU_CELL_WIDTH*NU_CELL_HEIGHT);

    // Clamp m_position to cell corner
    int x = position.m_x;
    int y = position.m_y;
    if (x < 0)
        x -= (NU_CELL_WIDTH-1);
    if (y < 0)
        y -= (NU_CELL_HEIGHT-1);
    m_position = Vec2i(x, y);
}

Cell::~Cell()
{
    for (int i=0; i<NU_MAX_LAYERS; i++)
    for (int y=0; y<NU_CELL_HEIGHT; y++)
    for (int x=0; x<NU_CELL_WIDTH; x++)
        if (m_blocks[i][y][x])
            delete m_blocks[i][y][x];
}

void Cell::serialize(std::ofstream* stream)
{
    char tag = World::WORLD_TAG_CELL;

    stream->write(&tag, 1);
    m_position.serialize(stream);

    for (int i=0; i<NU_MAX_LAYERS; i++)
    for (int y=0; y<NU_CELL_HEIGHT; y++)
    for (int x=0; x<NU_CELL_WIDTH; x++)
    {
        Block* block = m_blocks[i][y][x];
        if (block)
        {
            // TODO [sampo] why aren't these handled in the block serializer?
            tag = World::WORLD_TAG_BLOCK;
            stream->write(&tag, 1);
            stream->write((char*)&i, 4);
            stream->write((char*)&y, 4);
            stream->write((char*)&x, 4);
            block->serialize(stream);
        }
    }
}

void Cell::deserialize(std::ifstream* stream, int version)
{
    memset(m_blocks, 0, sizeof(Block*)*NU_MAX_LAYERS*NU_CELL_WIDTH*NU_CELL_HEIGHT);

    if (version == World::VERSION_INITIAL)
    {
        stream->read((char*)&m_position.m_x, 4);
        stream->read((char*)&m_position.m_y, 4);
    }
    else
    {
        m_position.deserialize(stream);
    }

    char tag = -1;
    stream->read((char*)&tag, 1);
    while (tag == World::WORLD_TAG_BLOCK)
    {
        int layer, blockX, blockY;

        // TODO [sampo] why aren't these handled in the block serializer?
        stream->read((char*)&layer, 4);
        stream->read((char*)&blockY, 4);
        stream->read((char*)&blockX, 4);
        
        NU_ASSERT(layer >= 0 && layer < NU_MAX_LAYERS);
        NU_ASSERT(blockY >= 0 && blockY < NU_CELL_HEIGHT);
        NU_ASSERT(blockX >= 0 && blockX< NU_CELL_WIDTH);

        Block* block = new Block();
        m_blocks[layer][blockY][blockX] = block;
        block->deserialize(stream);

        stream->read((char*)&tag, 1);
    }

    stream->seekg(-1, std::ios_base::cur); 
}

void Cell::insertBlock(char type, Vec3i location)
{
    // translate coordinates to cell local coordinate space
    int x = location.m_x;
    int y = location.m_y;
    if (x<0)
    {
        x *= -1;
        x -= 1;
    }
    if (y<0)
    {
        y *= -1;
        y -= 1;
    }
    x = x % NU_CELL_WIDTH;
    y = y % NU_CELL_HEIGHT;
    
    int layer = location.m_z;
    if (m_blocks[layer][y][x])
        delete m_blocks[layer][y][x];
    m_blocks[layer][y][x] = new Block(type, Vec3i(x, y, layer));
}

void Cell::insertBlock(Block* block)
{
    Vec3i location = block->getLocation();

    // translate coordinates to cell local coordinate space
    int x = location.m_x;
    int y = location.m_y;
    if (x<0)
    {
        x *= -1;
        x -= 1;
    }
    if (y<0)
    {
        y *= -1;
        y -= 1;
    }
    x = x % NU_CELL_WIDTH;
    y = y % NU_CELL_HEIGHT;

    int layer = location.m_z;
    if (m_blocks[layer][y][x])
        delete m_blocks[layer][y][x];
    m_blocks[layer][y][x] = block; 
}

unsigned int Cell::indexAtLocation(Vec3i location)
{
    int x = location.m_x;
    int y = location.m_y;
    if (x < 0)
        x -= (NU_CELL_WIDTH-1);
    if (y < 0)
        y -= (NU_CELL_HEIGHT-1);
    x /= NU_CELL_WIDTH;
    y /= NU_CELL_HEIGHT;

    short int sx = (short int)x;
    unsigned short int usx = *((unsigned short int*)&sx);
    short int sy = (short int)y;
    unsigned short int usy = *((unsigned short int*)&sy);

    return ((usx & 0xffff)<<16) + (usy & 0xffff);
}

unsigned int Cell::getIndex()
{
    return indexAtLocation(Vec3i(m_position.m_x, m_position.m_y, 0));
}

void Cell::clearBlock(Vec3i position)
{
    // translate coordinates to cell local coordinate space
    int x = position.m_x;
    int y = position.m_y;
    if (x<0)
    {
        x *= -1;
        x -= 1;
    }
    if (y<0)
    {
        y *= -1;
        y -= 1;
    }
    x = x % NU_CELL_WIDTH;
    y = y % NU_CELL_HEIGHT;

    if (m_blocks[position.m_z][y][x])
    {
        delete m_blocks[position.m_z][y][x];
        m_blocks[position.m_z][y][x] = NULL;
    }
}


const Block* Cell::getBlock(Vec3i location)
{
    // translate coordinates to cell local coordinate space
    int x = location.m_x;
    int y = location.m_y;
    if (x<0)
    {
        x *= -1;
        x -= 1;
    }
    if (y<0)
    {
        y *= -1;
        y -= 1;
    }
    x = x % NU_CELL_WIDTH;
    y = y % NU_CELL_HEIGHT;

    return m_blocks[location.m_z][y][x];
}