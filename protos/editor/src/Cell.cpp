#include <assert.h>
#include "Cell.h"
#include "textures.h"
#include "gl_utils.h"
#include "Tilemap.h"

extern TextureManager *textures;

Cell::Cell() 
{
}

Cell::Cell(int nl, int cs) {
    cellSize = cs;
    numLayers = nl;
    x = y = 0.0f;

    mapData = new unsigned char*[numLayers];
}

// reset layer0=random, other layers=blank
void Cell::reset() {
    for (int i = 0; i < numLayers; i++) {
	mapData[i] = new unsigned char[cellSize*cellSize];
	for (int j = 0; j < cellSize*cellSize; j++)
	    mapData[i][j] = i == 0 ? 1+(rand()%(BT_LASTBLOCK-1)) : BT_BLANK;
    }
}

int Cell::getBlockAt(int x, int y, int layer) {
    assert(x >= 0 && y >= 0 && layer >= 0);

    int ofs = (y * cellSize + x) * 3;
    int r = mapData[layer][ofs++];
    int g = mapData[layer][ofs++];
    int b = mapData[layer][ofs++];

    return (r << 16) | (g << 8) | b;
}

void Cell::setBlock(int x, int y, int layer, int block) 
{
    //assert(block >= 0 && block < BT_LASTBLOCK);
    assert(x >= 0 && x < cellSize && y >= 0 && y < cellSize);
    assert(layer >= 0 && layer < numLayers);

    mapData[layer][y*cellSize+x] = block;
}

void Cell::renderRaw(Tilemap* tilemap) 
{
    for (int layer = 0; layer < numLayers; layer++)
    {
	int ofs = 0;
	for (int j = 0; j < cellSize; j++) 
	{
	    for (int i = 0; i < cellSize; i++, ofs++) 
	    {
		unsigned char block = mapData[layer][ofs];

		glPushMatrix();
		glTranslatef(x+i, y-j, (layer == 0) ? 0.0f : layer-0.5f);
		if (layer == 0)
		    tilemap->texturedPlane(block, 1.0);
		//else 
		//    texturedCube(1.0);
		glPopMatrix();
	    }
	}    
    }
}

GLuint Cell::getTextureForBlock(int block) 
{
    if (block == BT_WATER) return textures->get("water");
    if (block == BT_GRASS) return textures->get("grass"); 
    if (block == BT_ROCK) return textures->get("rock"); 
    if (block == BT_WOOD) return textures->get("wood"); 
}

void Cell::move(float _x, float _y) 
{
    x = _x;
    y = _y;
}

void Cell::writeToFile(FILE* fp)
{
    for (int i = 0; i < numLayers; i++)
	fwrite(mapData[i], sizeof(unsigned char), cellSize*cellSize, fp);
}

void Cell::readFromFile(FILE* fp)
{
    for (int i = 0; i < numLayers; i++)
	fread(mapData[i], sizeof(unsigned char), cellSize*cellSize, fp);
}
