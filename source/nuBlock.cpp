#include "nuBlock.h"

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
{}

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
}

void Block::render() const
{
    // TODO [sampo] move the gl calls to graphics.cpp
	glPushMatrix();
    glColor4f(1, 0, 0, 1);
    glTranslatef((GLfloat)m_blockCoords.m_x, (GLfloat)m_blockCoords.m_y, 0);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1, -1, 0.0f);
    glVertex3f( 1, -1, 0.0f);
    glVertex3f( 1,  1, 0.0f);
    glVertex3f(-1,  1, 0.0f);
    glEnd();
    glPopMatrix();
}
