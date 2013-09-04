#pragma once

namespace Nultima
{

class Vec3
{
public:
    Vec3() : m_x(0), m_y(0), m_z(0) {}
    Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
    ~Vec3() {}

    Vec3 operator+ (Vec3);

    float m_x;
    float m_y;
    float m_z;
};

class Vec3ui
{
public:
    Vec3ui() : m_x(0), m_y(0), m_z(0) {}
    Vec3ui(unsigned int x, unsigned int y, unsigned int z) : m_x(x), m_y(y), m_z(z) {}
    ~Vec3ui() {}

    unsigned int m_x;
    unsigned int m_y;
    unsigned int m_z;
};

};