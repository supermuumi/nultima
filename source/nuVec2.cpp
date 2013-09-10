#include "nuVec2.h"

using namespace Nultima;

void Vec2i::serialize(std::ofstream* stream)
{
    stream->write((char*)&m_x, 4);
    stream->write((char*)&m_y, 4);
}

void Vec2i::deserialize(std::ifstream* stream)
{
    stream->read((char*)&m_x, 4);
    stream->read((char*)&m_y, 4);
}

Vec2i Vec2i::operator+(Vec2i v) 
{
    Vec2i ret;
    ret.m_x = m_x + v.m_x;
    ret.m_y = m_y + v.m_y;
    return ret;
}

Vec2i Vec2i::operator-(Vec2i v) 
{
    Vec2i ret;
    ret.m_x = m_x - v.m_x;
    ret.m_y = m_y - v.m_y;
    return ret;
}

Vec2i Vec2i::operator/(Vec2i v) 
{
    Vec2i ret;
    ret.m_x = m_x / v.m_x;
    ret.m_y = m_y / v.m_y;
    return ret;
}
