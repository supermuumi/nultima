#pragma once

#include "nuDefs.h"
#include "nuBlock.h"

#include <fstream>

namespace Nultima
{

class Block;

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

private:
    int     m_x;
    int     m_y;
    Block*  m_blocks[NU_MAX_LAYERS][NU_CELL_HEIGHT][NU_CELL_WIDTH];
};

};
