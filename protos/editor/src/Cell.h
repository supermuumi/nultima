#ifndef __CELL_H__
#define __CELL_H__

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include <stdio.h>

#define CELL_MAX_LAYERS 2 // 

// pixel-to-block mapping (RGB values in .png denote block type)
// TODO these should probably be in a separate Block.h

#define BLOCK_WATER 0x0000ff
#define BLOCK_GRASS 0x00ff00
#define BLOCK_ROCK  0x808080
#define BLOCK_WOOD  0xff8000

enum BlockType
{
    BT_BLANK,

    BT_WATER,
    BT_GRASS,
    BT_ROCK,
    BT_WOOD,

    BT_LASTBLOCK
};

class Cell {
 public:
    Cell();
    Cell(int nl, int cs);

    void load(char *fname, int layer); // stream in, generate map
    void unload(); // stream out
    void render();
    void renderRaw();
    void move(float, float);
    void reset();

    int getBlockAt(int x, int y, int layer);
    void setBlock(int x, int y, int layer, int block);
    GLuint getTextureForBlock(int);

    void readFromFile(FILE* fp);
    void writeToFile(FILE* fp);
    
    int cellSize;
    int numLayers;
    float x, y;
    char *fname;
    bool inMemory;
    GLuint *displayList; // display list - TODO replace with VBO or something more.. contemporary :)

    unsigned char **mapData;
};


#endif
