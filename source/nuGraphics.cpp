#include "nuGraphics.h"
#include "nuDefs.h"
#include "nuGlut.h"

#if NU_OS == NU_OS_WINDOWS
#   include <Windows.h>
#endif


#include "GL/glew.h"

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


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

    glewInit();
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

void Graphics::translate(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void Graphics::pushMatrix()
{
    glPushMatrix();
}

void Graphics::popMatrix()
{
    glPopMatrix();
}

unsigned int Graphics::createIndexBuffer(unsigned int* tris, int numTris)
{
    GLuint indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*numTris*3, tris, GL_STATIC_DRAW);
    return indexBufferObject;
}

void Graphics::bindIndexBuffer(unsigned int buffer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)buffer);
}

unsigned int Graphics::createVertexBuffer(float* verts, int numVerts)
{
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, numVerts*sizeof(float)*3, verts, GL_STATIC_DRAW);
    return vertexBufferObject;
}

void Graphics::bindVertexBuffer(unsigned int buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)buffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(float)*3, 0);
}

void Graphics::drawElements(int count)
{
    glDrawElements(GL_TRIANGLES, count*3, GL_UNSIGNED_INT, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
}
