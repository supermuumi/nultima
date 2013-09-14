#include "nuVec3.h"
#include "nuLight.h"

using namespace Nultima;

Light::Light(Vec3 pos, Vec3 ambient, Vec3 diffuse, Vec3 specular) :
    m_pos(pos),
    m_ambient(ambient),
    m_diffuse(diffuse),
    m_specular(specular)
{
}

void Light::getAmbient(float *array)
{
    return m_ambient.getFloats(array);
}

void Light::getDiffuse(float *array)
{
    return m_diffuse.getFloats(array);
}

void Light::getSpecular(float *array)
{
    return m_specular.getFloats(array);
}
