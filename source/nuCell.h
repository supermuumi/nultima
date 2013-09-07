#pragma once

#include "nuDefs.h"
#include "nuBlock.h"
#include "nuVec2.h"

#include <fstream>

namespace Nultima
{

class Cell
{
public:
    Cell() {};
    Cell(int x, int y);
    ~Cell();

    const Block*    getBlock    (int x, int y, int layer) { return m_blocks[layer][y][x]; }
    void            insertBlock (char type, Vec2i coords, int layer);

    void            beginRendering  ();
    void            endRendering    ();

    void            serialize   (std::ofstream* stream);
    void            deserialize (std::ifstream* stream);

    Vec2i getPosition() { return Vec2i(m_x, m_y); }

private:
    int     m_x;
    int     m_y;
    Block*  m_blocks[NU_MAX_LAYERS][NU_CELL_HEIGHT][NU_CELL_WIDTH];
};

};
