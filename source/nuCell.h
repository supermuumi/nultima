#pragma once

#include "nuDefs.h"
#include "nuBlock.h"
#include "nuVec2.h"
#include "nuVec3.h"

#include <fstream>

namespace Nultima
{

class Cell
{
public:
    Cell() {};
    Cell(Vec2i position);
    ~Cell();

    // TODO [sampo] Use Vec3
    const Block*    getBlock    (Vec3i location) { return m_blocks[location.m_z][location.m_y][location.m_x]; }
    void            insertBlock (char type, Vec3i location);
    void            insertBlock (Block* block);

    void            clearBlock  (Vec3i location);

    void            serialize   (std::ofstream* stream);
    void            deserialize (std::ifstream* stream, int version);

    unsigned int    getIndex    ();

    static unsigned int indexAtLocation (Vec3i location);

private:
    Vec2i m_position;
    Block*  m_blocks[NU_MAX_LAYERS][NU_CELL_HEIGHT][NU_CELL_WIDTH];
};

};
