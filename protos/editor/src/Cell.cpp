#include <assert.h>§
#include "Cell.h"
#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"
#include "textures.h"
#include "gl_utils.h"

extern TextureManager *textures;

Cell::Cell() {
    numLayers = 1;
    displayList = new GLuint[numLayers];
    inMemory = false;
    x = y = 0.0f;

    mapData = new unsigned char*[CELL_MAX_LAYERS];
}

Cell::Cell(int n) {
    numLayers = n;
    displayList = new GLuint[numLayers];
    inMemory = false;
    x = y = 0.0f;

    mapData = new unsigned char*[CELL_MAX_LAYERS];
}

// reset layer0=random, other layers=blank
void Cell::reset() {
    for (int i = 0; i < numLayers; i++) {
	mapData[i] = new unsigned char[CELL_SIZE*CELL_SIZE];
	for (int j = 0; j < CELL_SIZE*CELL_SIZE; j++)
	    mapData[i][j] = i == 0 ? 1+(rand()%(BT_LASTBLOCK-1)) : BT_BLANK;
    }
}

int Cell::getBlockAt(int x, int y, int layer) {
    assert(x >= 0 && y >= 0 && layer >= 0);

    int ofs = (y * CELL_SIZE + x) * 3;
    int r = mapData[layer][ofs++];
    int g = mapData[layer][ofs++];
    int b = mapData[layer][ofs++];

    return (r << 16) | (g << 8) | b;
}

void Cell::setBlock(int x, int y, int layer, int block) 
{
    assert(block >= 0 && block < BT_LASTBLOCK);
    assert(x >= 0 && x < CELL_SIZE && y >= 0 && y < CELL_SIZE);
    assert(layer >= 0 && layer < numLayers);

    mapData[layer][y*CELL_SIZE+x] = block;
}

void Cell::load(char *fname, int layer) {
    int mapWidth, mapHeight, bpp;    

    assert(layer >= 0 && layer < numLayers);
    
    mapData[layer] = stbi_load(fname, &mapWidth, &mapHeight, &bpp, 3);

    assert(mapWidth==CELL_SIZE && mapHeight==CELL_SIZE);

    displayList[layer] = glGenLists(1);
    glNewList(displayList[layer], GL_COMPILE);

    for (int y = 0; y < CELL_SIZE; y++) {
	for (int x = 0; x < CELL_SIZE; x++) {

	    int p = getBlockAt(x, y, layer);

	    switch (p) {

		case BLOCK_WATER: 
		    glBindTexture(GL_TEXTURE_2D, textures->get("water")); 
		    break;
		case BLOCK_GRASS: 
		    glBindTexture(GL_TEXTURE_2D, textures->get("grass")); 
		    break;
		case BLOCK_ROCK: 
		    glBindTexture(GL_TEXTURE_2D, textures->get("rock")); 
		    break;
		case BLOCK_WOOD: 
		    glBindTexture(GL_TEXTURE_2D, textures->get("wood")); 
		    break;
		    
	    default: continue;
	    }

	    // TODO should merge identical neighbors to reduce calls
	    glPushMatrix();
	    glTranslatef((float)x, (float)y, 0.0f);
	    if (layer == 0)
		texturedPlane(1.0);
	    else 
		texturedCube(1.0);
	    glPopMatrix();
	}
    }

    glEndList();

    inMemory = true;
}

void Cell::unload() {
    for (int i = 0; i < numLayers; i++) {
	glDeleteLists(displayList[i], 1);
	stbi_image_free(mapData[i]);
    }
    inMemory = false;
}

void Cell::render() {
    // TODO should assert?
    if (inMemory) {
	for (int i = 0; i < numLayers; i++) {
	    glPushMatrix();
	    glTranslatef(x, y, (i == 0) ? 0.0f : i-0.5f);
	    glCallList(displayList[i]);
	    glPopMatrix();
	}
    }
}

void Cell::renderRaw() 
{
    for (int layer = 0; layer < numLayers; layer++)
    {
	int ofs = 0;
	for (int j = 0; j < CELL_SIZE; j++) 
	{
	    for (int i = 0; i < CELL_SIZE; i++, ofs++) 
	    {
		unsigned char block = mapData[layer][ofs];
		if (block == BT_BLANK) 
		    continue;

		glBindTexture(GL_TEXTURE_2D, getTextureForBlock(block));
		glPushMatrix();
		glTranslatef(x+i, y-j, (layer == 0) ? 0.0f : layer-0.5f);
		if (layer == 0)
		    texturedPlane(1.0);
		else 
		    texturedCube(1.0);
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

void Cell::move(float _x, float _y) {
    x = _x;
    y = _y;
}
