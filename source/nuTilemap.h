#pragma once

#include <string>
#include <vector>
#include <map>

namespace Nultima
{

class Tilemap
{
public:
    Tilemap();
    ~Tilemap();

    void           load(std::string fname);
    unsigned int   getTexture(int id);

private:
    int                       m_tileSize;

    std::vector<unsigned int> m_tiles;
};

}; // namespace
