#pragma once

#include <string>
#include <vector>
#include <map>

namespace Nultima
{

class Tilemap
{
public:

    typedef struct 
    {
        float x1;
        float y1;
        float x2;
        float y2;
    } TilemapTexture;

    Tilemap();
    ~Tilemap();

    void           load(std::string fname);
    unsigned int   getTilemapId();
    TilemapTexture getTexture(std::string id);

private:
    void           createTexture();

    unsigned int                m_textureId;
    int                         m_tileSize;
    int                         m_tilemapWidth;
    int                         m_tilemapHeight;
    int                         m_tilemapBpp;
    int                         m_tilesPerLine;
//    unsigned char*              m_tilemapData;

    std::string                 m_tilemapFile;
    std::vector<TilemapTexture> m_tiles;
    std::map<std::string,int>   m_tileMapping;
};

}; // namespace
