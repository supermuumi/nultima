#include "nuBlock.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuDefs.h"
#include "nuModel.h"

#if NU_OS == NU_OS_WINDOWS
#   include <windows.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace Nultima;

Block::Block(char type, Vec2i coords, int layer) :
    m_type(type),
    m_blockCoords(coords),
    m_layer(layer)
{
    determineModel();
    determineTexName();
}

void Block::moveTo(Vec2i v)
{
    m_blockCoords.m_x = v.m_x;
    m_blockCoords.m_y = v.m_y;
}

void Block::setLayer(int n)
{
    assert(n >= 0 && n < NU_MAX_LAYERS);
    m_layer = n;
    determineModel();
}

void Block::setType(char type)
{
    // TODO set type
    m_type = type;
    determineTexName();
}

void Block::determineModel()
{
    Context* context = Context::get();
    if (m_layer == 0)
        m_model = context->getModel(Model::UNIT_PLANE);
    else
        m_model = context->getModel(Model::UNIT_BOX);
}

void Block::determineTexName()
{
    switch (m_type)
    {
    case GRASS:
        m_texName = "grass";
        break;
    case WATER:
        m_texName = "water";
        break;
    case ROCK:
        m_texName = "rock";
        break;
    default:
        NU_ASSERT(!"Unkown block type");
    }
}

void Block::serialize(std::ofstream* stream)
{
    stream->write(&m_type, 1);
    m_blockCoords.serialize(stream);
    stream->write((char*)&m_layer, 4);
}

void Block::deserialize(std::ifstream* stream)
{
    stream->read(&m_type, 1);
    m_blockCoords.deserialize(stream);
    stream->read((char*)&m_layer, 4);

    determineModel();
    determineTexName();
}

void Block::render() const
{
    Context* context = Context::get();
    Graphics* graphics = context->getGraphics();

    graphics->pushMatrix();
    graphics->translate((float)m_blockCoords.m_x, (float)m_blockCoords.m_y, m_layer>0 ? (float)(m_layer-1) : 0);
    graphics->bindTexture(context->getTexture(m_texName));

    m_model->render();

    graphics->popMatrix();
}
