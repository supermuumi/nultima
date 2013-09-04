#include "nuBlock.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuDefs.h"
#include "nuModel.h"

#if NU_OS == NU_OS_WINDOWS
#   include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

using namespace Nultima;

Block::Block(char type, Vec2i coords, int layer) :
    m_type(type),
    m_blockCoords(coords),
    m_layer(layer)
{
    determineModel();
}

void Block::determineModel()
{
    Context* context = Context::get();
    if (m_layer == 0)
        m_model = context->getModel(Model::UNIT_PLANE);
    else
        m_model = context->getModel(Model::UNIT_BOX);
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
}

void Block::render() const
{
    Graphics* graphics = Context::get()->getGraphics();
	graphics->pushMatrix();
    graphics->translate((float)m_blockCoords.m_x, (float)m_blockCoords.m_y, 0);

    switch (m_type)
    {
    case GRASS:
        glColor4f(0, 1, 0, 1);
        break;

    case WATER:
        glColor4f(0, 0, 1, 1);
        break;

    default:
        NU_ASSERT(!"Unkonwn block type");
    }

#if 1
    m_model->render();
#else
    // TODO [sampo] Vertex buffer
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0.0f);
    glVertex3f(1, 0, 0.0f);
    glVertex3f(1, 1, 0.0f);
    glVertex3f(0, 1, 0.0f);
    glEnd();
#endif

    graphics->popMatrix();
}
