#include <assert.h>
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
}

Cell::Cell(int n) {
    numLayers = n;
    displayList = new GLuint[numLayers];
    inMemory = false;
    x = y = 0.0f;
}
/*
Cell::~Cell() {
    delete displayList;
}
*/
void Cell::load(char *fname, int layer) {
    int mapWidth, mapHeight, bpp;    
    unsigned char *mapData;

    assert(layer >= 0 && layer < numLayers);
    
    mapData = stbi_load(fname, &mapWidth, &mapHeight, &bpp, 3);

    assert(mapWidth==CELL_SIZE && mapHeight==CELL_SIZE);

    displayList[layer] = glGenLists(1);
    glNewList(displayList[layer], GL_COMPILE);

    for (int y = 0, ofs = 0; y < CELL_SIZE; y++) {
	for (int x = 0; x < CELL_SIZE; x++) {
	    int r = mapData[ofs++];
	    int g = mapData[ofs++];
	    int b = mapData[ofs++];
	    int p = (r << 16) | (g << 8) | b;

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
    stbi_image_free(mapData);
    glEndList();

    inMemory = true;
}

void Cell::unload() {
    for (int i = 0; i < numLayers; i++)
	glDeleteLists(displayList[i], 1);
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

void Cell::move(float _x, float _y) {
    x = _x;
    y = _y;
}
