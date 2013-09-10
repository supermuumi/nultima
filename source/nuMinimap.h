#pragma once

#include "nuVec2.h"

#include <vector>

namespace Nultima
{

class Object;
class UiQuad;
class Cell;
class World;

class Minimap
{
public:
    Minimap(World* world);
    ~Minimap();

    void            render              ();
    void            update              ();
    unsigned int    determineColor      (Vec2i coord, int size);
    void            getScreenLocation   (int& x, int& y, int& width, int& height);
    void            getWorldMinMax      (Vec2i& min, Vec2i& max);

private:
    std::vector<unsigned int>   m_pixels;
    UiQuad*                     m_quad;
    unsigned int                m_texture;
    World*                      m_world;
};

};
