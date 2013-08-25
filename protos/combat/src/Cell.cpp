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

void Cell::load(int layer) {
    assert(layer >= 0 && layer < numLayers);
    
    displayList[layer] = glGenLists(1);
    glNewList(displayList[layer], GL_COMPILE);

    for (int y = 0; y < CELL_SIZE; y++) {
	for (int x = 0; x < CELL_SIZE; x++) {

		glBindTexture(GL_TEXTURE_2D, textures->get("grass")); 

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

void Cell::unload()
{
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
