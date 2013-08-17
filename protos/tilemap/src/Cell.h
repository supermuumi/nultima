#ifndef __CELL_H__
#define __CELL_H__

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define CELL_SIZE 64 // each cell is CELL_SIZExCELL_SIZE blocks, each block is 1.0 in size

// pixel-to-block mapping (RGB values in .png denote block type)
// TODO these should probably be in a separate Block.h
#define BLOCK_WATER 0x0000ff
#define BLOCK_GRASS 0x00ff00
#define BLOCK_ROCK  0x808080
#define BLOCK_WOOD  0xff8000

class Cell {
 public:
    Cell();
    Cell(int n);
    //    ~Cell();

    void load(char *fname, int layer); // stream in, generate map
    void unload(); // stream out
    void render();
    void move(float, float);

    int numLayers;
    float x, y;
    float r, g, b;
    char *fname;
    bool inMemory;
    GLuint *displayList; // display list - TODO replace with VBO or something more.. contemporary :)

};


#endif
