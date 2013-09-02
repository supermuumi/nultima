#include "nuBlock.h"

using namespace Nultima;

void Block::serialize(std::ofstream* stream)
{
    stream->write(&m_type, 1);
}

void Block::deserialize(std::ifstream* stream)
{
    stream->read(&m_type, 1);
}
