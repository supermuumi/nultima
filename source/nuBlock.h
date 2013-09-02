#pragma once

#include "nuObject.h"

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
    Block(char type) : m_type(type) {}
    ~Block() {}

    void serialize      (std::ofstream* stream);
    void deserialize    (std::ifstream* stream);

private:
    char    m_type;
};

};