#pragma once

#include <fstream>

namespace Nultima
{

class Vec2
{
public:
    Vec2() : m_x(0), m_y(0) {}
    Vec2(float x, float y) : m_x(x), m_y(y) {}
    ~Vec2() {}

    float m_x;
    float m_y;
};

class Vec2i
{
public:
    Vec2i() : m_x(0), m_y(0) {};
    Vec2i(int x, int y) : m_x(x), m_y(y) {};
    ~Vec2i() {}

    Vec2i operator+      (Vec2i);
    Vec2i operator-      (Vec2i);
    Vec2i operator/      (Vec2i);

    void  serialize      (std::ofstream* stream);
    void  deserialize    (std::ifstream* stream);

    int m_x;
    int m_y;
};

};
