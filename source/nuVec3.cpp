#include "nuVec3.h"

using namespace Nultima;

Vec3 Vec3::operator+ (Vec3 v)
{
    Vec3 ret;
    ret.m_x = m_x + v.m_x;
    ret.m_y = m_y + v.m_y;
    ret.m_z = m_z + v.m_z;
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

