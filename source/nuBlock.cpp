#include "nuBlock.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuDefs.h"
#include "nuModel.h"
#include "nuTexManager.h"

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

Block::Block(char type, Vec3i location) :
    m_type(type),
    m_location(location)
{
    m_representation = AUTO;
    determineModel();
}

void Block::moveTo(Vec3i v)
{
    NU_ASSERT(v.m_x < NU_CELL_WIDTH && v.m_y < NU_CELL_HEIGHT);
    m_location.m_x = v.m_x;
    m_location.m_y = v.m_y;
    m_location.m_z = v.m_z;
    determineModel();
}

void Block::setType(char type)
{
    m_type = type;
}

void Block::determineModel()
{
    Context* context = Context::get();
    if (m_location.m_z == 0)
        m_model = context->getModel(Model::UNIT_PLANE);
    else if (m_representation == AUTO)
        m_model = context->getModel(Model::UNIT_BOX);
    else
        m_model = context->getModel(m_representation == BLOCK ? Model::UNIT_BOX : Model::UNIT_PLANE);
}

void Block::serialize(std::ofstream* stream)
{
    stream->write(&m_type, 1);
    m_location.serialize(stream);
}

void Block::deserialize(std::ifstream* stream)
{
    stream->read(&m_type, 1);
    m_location.deserialize(stream);

    determineModel();
}

void Block::render() const
{
    Context* context = Context::get();
    Graphics* graphics = context->getGraphics();

    graphics->pushMatrix();
    graphics->translate((float)m_location.m_x, (float)m_location.m_y, m_location.m_z>0 ? (float)(m_location.m_z-1) : 0);
    graphics->bindTexture(context->getTilemapTexture(m_type));

    m_model->render();

    graphics->popMatrix();
}

void Block::setRepresentation(NuUInt8 type)
{
    m_representation = type;
    determineModel();
}
