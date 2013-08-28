#ifndef __WORLD_H__
#define __WORLD_H__

#include <iostream>
#include <vector>
#include "Cell.h"
#include "camera.h"

using namespace std;

class World
{
public:
    World(int, int, int);

    friend ostream& operator<<(ostream &os, const World& w);

    Cell* getCellAt(int x, int y);
    int getBlockAt(int x, int y, int layer);
    void setBlockAt(int x, int y, int layer, int block);
    void render(Camera* cam);

    int getWidth();
    int getHeight();

private:
    int m_cellWidth;
    int m_cellHeight;
    int m_cellSize;
    vector<Cell*> m_cells;

};

#endif
