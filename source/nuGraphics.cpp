#include "nuGraphics.h"
#include "nuDefs.h"
#include "nuGlut.h"

#if NU_OS == NU_OS_WINDOWS
#   include <Windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

using namespace Nultima;

Graphics::Graphics(int wWidth, int wHeight) :
    m_wWidth(wWidth),
    m_wHeight(wHeight)
{
}

Graphics::~Graphics()
{
    NU_ASSERT(!m_GLUT);
}

void Graphics::init(int argc, char** argv)
{
    m_GLUT = new GLUT();
    m_GLUT->init(argc, argv, m_wWidth, m_wHeight);
}

void Graphics::deinit()
{
    m_GLUT->deinit();
    delete m_GLUT;
    m_GLUT = NULL;
}

void Graphics::resize(int x, int y)
{
    m_wWidth = x; 
    m_wHeight = y;

    glViewport(0, 0, x, y);

    setViewProjection();
}

void Graphics::setViewProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 70.0, (GLfloat)(m_wWidth)/(GLfloat)(m_wHeight), 0.1f, 50000.0 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Graphics::clear()
{
    glClearColor(0.1f, 0.2f, 0.4f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Graphics::swap()
{
    m_GLUT->swap();
}

void Graphics::lookAt(Vec3 pos, Vec3 center, Vec3 up)
{
    gluLookAt(
        pos.m_x, pos.m_y, pos.m_z,
        center.m_x, center.m_y, center.m_z,
        up.m_x, up.m_y, up.m_z);
}