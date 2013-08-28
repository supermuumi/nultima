#ifndef __WORLD_H__
#define __WORLD_H__

#include <iostream>
#include <vector>
#include "Cell.h"
#include "camera.h"

using namespace std;

#define WORLD_SIZE 4
#define WORLD_CELLSIZE 16

class World
{
public:
    World();
    World(int, int, int);
    ~World();

    void save(char *fname);
    void load(char *fname);


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
    bool m_worldInvalid;
    vector<Cell*> m_cells;

};

#endif
