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
