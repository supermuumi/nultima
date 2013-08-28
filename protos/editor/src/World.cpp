#include <iostream>
#include "World.h"
#include "Cell.h"

using namespace std;

World::World(int w, int h, int cs) 
{
    m_cellWidth = w;
    m_cellHeight = h;
    m_cellSize = cs;

    for (int j = 0; j < m_cellHeight; j++)
    {
	for (int i = 0; i < m_cellWidth; i++)
	{
	    Cell *c = new Cell(2, m_cellSize);
	    c->move((float)i*m_cellSize, (float)-j*m_cellSize);
	    c->reset();
	    m_cells.push_back(c);
	}
    }
}

int World::getWidth()
{
    return m_cellWidth * m_cellSize;
}

int World::getHeight()
{
    return m_cellHeight * m_cellSize;
}

Cell* World::getCellAt(int x, int y) 
{
    int idx = y/m_cellSize*m_cellWidth + x/m_cellSize;
    printf("cellAt(%d, %d) = %d\n", x, y, idx);
    return m_cells.at(idx);
}

int World::getBlockAt(int x, int y, int layer)
{
    Cell *c = getCellAt(x, y);
    return c->getBlockAt(x, y, layer);
}

void World::setBlockAt(int x, int y, int layer, int block)
{
    Cell *c = getCellAt(x, y);
    c->setBlock(x % m_cellSize, y % m_cellSize, layer, block);
}

void World::render(Camera *cam)
{
    for (std::vector<Cell*>::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
    {	
	if (cam->cubeInFrustum((*it)->x+m_cellSize/2.0f, 
			       (*it)->y+m_cellSize/2.0f, 
			       0.0f, m_cellSize/2.0f))
	{
	    (*it)->renderRaw();
	}
    }
}

ostream& operator<<(ostream &os, const World& w) 
{
    os << "123";
    return os;
}
