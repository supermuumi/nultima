#pragma once

#include "nuObject.h"
#include "nuDefs.h"

namespace Nultima
{

class Model;

class UiQuad : public Object
{
public:

    UiQuad(int x, int y, int width, int height, unsigned int texture);
    ~UiQuad() {};

    void serialize      (std::ofstream* stream) {NU_UNREF(stream);}
    void deserialize    (std::ifstream* stream) {NU_UNREF(stream);}

    void render         () const;

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    unsigned int m_texture;
};

};