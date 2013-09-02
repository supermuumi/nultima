#pragma once

namespace Nultima
{

class Block;

class Cell
{
public:
    Cell();
    ~Cell();

    const Block* getBlock(int x, int y, int layer) { return m_blocks[layer][y][x]; }

private:
    Block* m_blocks[NU_MAX_LAYERS][NU_CELL_WIDTH][NU_CELL_HEIGHT];
};

};
