#include "nuTilemap.h"
#include "nuFileUtils.h"
#include "document.h"
#include <string>
#include <iostream>

#if NU_OS == NU_OS_WINDOWS
#   include <Windows.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

using namespace Nultima;

Tilemap::Tilemap()
{
    m_tiles.resize(512);
}

Tilemap::~Tilemap()
{
}

void Tilemap::load(std::string fname)
{
    rapidjson::Document doc;

    char* jsonBlob = FileUtils::readFile(fname);
    assert(jsonBlob != NULL);
    doc.Parse<0>(jsonBlob);
    delete jsonBlob;
    assert(!doc.HasParseError());

    // parse config
    m_tileSize = doc["config"]["tilesize"].GetInt();
    m_tilemapFile = doc["config"]["tilemap"].GetString();

    // load tilemap texture
    createTexture();

    // process tile info (needs config & actual tilemap data)
    const rapidjson::Value& tiles = doc["tiles"];
    for (rapidjson::Value::ConstMemberIterator itr = tiles.MemberBegin(); itr != tiles.MemberEnd(); ++itr)
    {
        std::string textureId = itr->name.GetString();
        int idx = itr->value.GetInt();

	int x1 = idx%m_tilesPerLine * m_tileSize;
	int y1 = idx/m_tilesPerLine * m_tileSize;
	int x2 = x1+m_tileSize;
	int y2 = y1+m_tileSize;

	TilemapTexture textureInfo = {(float)x1/m_tilemapWidth, 
				      (float)y2/m_tilemapHeight, 
				      (float)x2/m_tilemapWidth,
				      (float)y1/m_tilemapHeight};
	m_tiles[idx] = textureInfo;
	m_tileMapping[textureId] = idx;
    }
}

// TODO use Nultima::Graphics here?
void Tilemap::createTexture()
{
    unsigned char* tilemapData; 

    tilemapData = stbi_load(m_tilemapFile.c_str(), &m_tilemapWidth, &m_tilemapHeight, &m_tilemapBpp, 0);
    std::cout << "tilemap = " << m_tilemapWidth << "x" << m_tilemapHeight << "x" << m_tilemapBpp << "\n";
    m_tilesPerLine = m_tilemapWidth / m_tileSize;

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_tilemapWidth, m_tilemapHeight,
		      GL_RGB, GL_UNSIGNED_BYTE, tilemapData);

    free(tilemapData);
}

unsigned int Tilemap::getTilemapId()
{
    return m_textureId;
}

Tilemap::TilemapTexture Tilemap::getTexture(std::string id)
{
    return m_tiles[m_tileMapping[id]];
}
