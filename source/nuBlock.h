#pragma once

#include "nuObject.h"
#include "nuVec2.h"

#include <fstream>

namespace Nultima
{

class Block : public Object
{
public:

    enum Type
    {
        GRASS = 0
    };

    Block() {}
    Block(char type, Vec2i coords, int layer);
    ~Block() {}

    void serialize      (std::ofstream* stream);
    void deserialize    (std::ifstream* stream);

    void render         () const;
private:
    char    m_type;
    Vec2i   m_blockCoords;
    int     m_layer;
};

};