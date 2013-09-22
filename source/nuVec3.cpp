#include "nuVec3.h"
#include <math.h>

using namespace Nultima;

Vec3 Vec3::operator+ (Vec3 v)
{
    Vec3 ret;
    ret.m_x = m_x + v.m_x;
    ret.m_y = m_y + v.m_y;
    ret.m_z = m_z + v.m_z;
    return ret;
}

void Vec3::getFloats(float* array)
{
    array[0] = m_x;
    array[1] = m_y;
    array[2] = m_z;
}

float Vec3::length()
{
    //return std::sqrtf(lengthSquared());
    return sqrtf(lengthSquared());
}

float Vec3::lengthSquared()
{
    return (float)(m_x*m_x + m_y*m_y + m_z*m_z);
}

Vec3 Vec3::operator-(Vec3 v)
{
    Vec3 ret;
    ret.m_x = m_x - v.m_x;
    ret.m_y = m_y - v.m_y;
    ret.m_z = m_z - v.m_z;
    return ret;
}

Vec3ui Vec3ui::operator+(Vec3ui v)
{
    Vec3ui ret;
    ret.m_x = m_x + v.m_x;
    ret.m_y = m_y + v.m_y;
    ret.m_z = m_z + v.m_z;
    return ret;
}

void Vec3i::serialize(std::ofstream* stream)
{
    stream->write((char*)&m_x, 4);
    stream->write((char*)&m_y, 4);
    stream->write((char*)&m_z, 4);
}

void Vec3i::deserialize(std::ifstream* stream)
{
    stream->read((char*)&m_x, 4);
    stream->read((char*)&m_y, 4);
    stream->read((char*)&m_z, 4);
}

Vec3i Vec3i::operator+(Vec3i v)
{
    Vec3i ret;
    ret.m_x = m_x + v.m_x;
    ret.m_y = m_y + v.m_y;
    ret.m_z = m_z + v.m_z;
    return ret;
}

Vec3i Vec3i::operator-(Vec3i v)
{
    Vec3i ret;
    ret.m_x = m_x - v.m_x;
    ret.m_y = m_y - v.m_y;
    ret.m_z = m_z - v.m_z;
    return ret;
}

float Vec3i::length()
{
    return sqrtf(lengthSquared());
}

float Vec3i::lengthSquared()
{
    return (float)(m_x*m_x + m_y*m_y + m_z*m_z);
}
