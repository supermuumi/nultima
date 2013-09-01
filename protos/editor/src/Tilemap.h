#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include <string>
#include <vector>
#include <map>
#include "libjson.h"

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

class Tilemap
{

struct TilemapTexture
{
    float x1;
    float y1;
    float x2;
    float y2;
};    


public:
    Tilemap();
    void load(char *fname);

    void parseConfig(const JSONNode& n);
    void parseTiles(const JSONNode& n);
    void createTexture();
    GLuint getTextureId();

    void texturedPlane(int idx, float size);

    vector<TilemapTexture> tiles;
    map<string,int> tileMapping;

    int tilemapWidth;
    int tilemapHeight;
    int tilemapBpp;
    int tileSize;
    int tilesPerLine;
    unsigned char* tilemapData;
    string tilemapFile;

    GLuint textureId;
};

#endif
