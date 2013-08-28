#include <iostream>
#include "World.h"
#include "Cell.h"
#include <stdio.h>

using namespace std;

World::World()
{
    m_worldInvalid = true;
}

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
    m_worldInvalid = false;
}

World::~World()
{
    while (m_cells.size())
    {
	Cell* c = m_cells.back();
	delete c;
	m_cells.pop_back();
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
    if (m_worldInvalid)
	return;

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

void World::save(char* fname) 
{
    m_worldInvalid = true;

    FILE *fp = fopen(fname, "wb");

    fwrite(&m_cellWidth, sizeof(int), 1, fp);
    fwrite(&m_cellHeight, sizeof(int), 1, fp);
    fwrite(&m_cellSize, sizeof(int), 1, fp);

    int n = 1;
    for (std::vector<Cell*>::iterator it = m_cells.begin(); it != m_cells.end(); ++it)
    {
	(*it)->writeToFile(fp);
	printf("wrote cell #%d\n", n);
	n++;
    }

    fclose(fp);

    m_worldInvalid = false;
}

void World::load(char* fname)
{
    m_worldInvalid = true;

    FILE *fp = fopen(fname, "rb");

    fread(&m_cellWidth, sizeof(int), 1, fp);
    fread(&m_cellHeight, sizeof(int), 1, fp);
    fread(&m_cellSize, sizeof(int), 1, fp);

    m_cells.clear();
    printf("loading %d cells...\n", m_cellWidth*m_cellHeight);
    for (int i = 0; i < m_cellWidth*m_cellHeight; i++)
    {
	printf("\tloading cell %d...\n", i+1);
	Cell* c = new Cell();
	c->readFromFile(fp);
	printf("\tadding cell %d to list...\n", i+1);
	m_cells.push_back(c);
	printf("\tdone\n");
    }
    printf("loading world done!\n");

    fclose(fp);

    m_worldInvalid = false;
}
