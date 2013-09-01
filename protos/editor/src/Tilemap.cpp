#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "Tilemap.h"
#include "libjson.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

using namespace std;

extern "C" {
    void             stbi_image_free (void *retval_from_stbi_load);
    unsigned char    *stbi_load      (char const *filename,     int *x, int *y, int *comp, int req_comp);
}


Tilemap::Tilemap()
{
    tileSize = 0;
    tilemapFile = "";

    tiles.resize(512);
}

void Tilemap::createTexture() 
{
    tilemapData = stbi_load(tilemapFile.c_str(), &tilemapWidth, &tilemapHeight, &tilemapBpp, 0);
    tilesPerLine = tilemapWidth / tileSize;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tilemapWidth, tilemapHeight,
		      GL_RGB, GL_UNSIGNED_BYTE, tilemapData);
}

// parse json/config, get filename and tilesize
void Tilemap::parseConfig(const JSONNode& n)
{
    JSONNode::const_iterator it = n.begin();
    while (it != n.end())
    {
	string nodeName = it->name();
	// might need stupid workaround, for some reason it->as_int() doesn't link!
	if (nodeName == "tilesize")
	    tileSize = atoi((it->as_string()).c_str());
	if (nodeName == "tilemap") {
	    tilemapFile = it->as_string();
	}
	++it;
    }

    createTexture();
}

// read through json/tiles
// for each tile, calculate index, crestore in a std::vector(?)
void Tilemap::parseTiles(const JSONNode& n)
{
    JSONNode::const_iterator it = n.begin();
    while (it != n.end())
    {
	string textureId = it->name();
	int idx = atoi((it->as_string()).c_str()); //it->as_int();
	
	int x1 = idx%tilesPerLine * tileSize;
	int y1 = idx/tilesPerLine * tileSize;
	int x2 = x1+tileSize;
	int y2 = y1+tileSize;

	TilemapTexture textureInfo = {(float)x1/tilemapWidth, 
				      (float)y1/tilemapHeight, 
				      (float)x2/tilemapWidth,
				      (float)y2/tilemapHeight};
	tiles[idx] = textureInfo;
	tileMapping[textureId] = idx;

	++it;
    }
}

GLuint Tilemap::getTextureId()
{
    return textureId;
}

void Tilemap::texturedPlane(int idx, float size)
{
    TilemapTexture ti = tiles[idx];

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(ti.x1, ti.y2);
    glVertex3f(-size/2.0f, -size/2.0f, 0.0f);
    glTexCoord2f(ti.x2, ti.y2);
    glVertex3f(size/2.0f, -size/2.0f, 0.0f);
    glTexCoord2f(ti.x2, ti.y1);
    glVertex3f(size/2.0f, size/2.0f, 0.0f);
    glTexCoord2f(ti.x1, ti.y1);
    glVertex3f(-size/2.0f, size/2.0f, 0.0f);
    glEnd();
}

void Tilemap::load(char* fname)
{
    // load tilemap.json (json)
    char* buffer = 0;
    long length;
    FILE* f = fopen("tilemap.json", "rb");

    if (f)
    {
	fseek (f, 0, SEEK_END);
	length = ftell(f);
	fseek (f, 0, SEEK_SET);
	buffer = new char [length+1];
	fread(buffer, 1, length, f);
	buffer[length] = 0;
	fclose(f);
    }
    JSONNode tilemap = libjson::parse(buffer);

    JSONNode::const_iterator it = tilemap.begin();
    while (it != tilemap.end()) 
    {
	string nodeName = it->name();
	if (nodeName == "config")
	    parseConfig(*it);
	else if (nodeName == "tiles")
	    parseTiles(*it);

	//increment the iterator
	++it;
    }


    // load tilemap w/ stbi_
    // calc tilesperline=tilemap_width/tilesize


    delete buffer;
}
