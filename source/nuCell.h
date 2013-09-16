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

    Vec2i           getPosition () {return m_position;}
    const Block*    getBlock    (Vec3i location);
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
