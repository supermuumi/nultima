#pragma once

#include "nuVec3.h"

namespace Nultima
{

class Light
{
public:
    Light(Vec3 pos, Vec3 ambient, Vec3 diffuse, Vec3 specular);
    ~Light() {}

    Vec3 getPosition() { return m_pos; }
    void getAmbient(float *array);
    void getDiffuse(float *array);
    void getSpecular(float *array);

//private:
    Vec3 m_pos;
    Vec3 m_ambient;
    Vec3 m_diffuse;
    Vec3 m_specular;
};

}; // namespace
