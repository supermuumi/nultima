#pragma once

namespace Nultima
{

class Vec2i
{
public:
    Vec2i() : m_x(0), m_y(0) {};
    Vec2i(int x, int y) : m_x(x), m_y(y) {};
    ~Vec2i() {}

    int m_x;
    int m_y;

};

};
