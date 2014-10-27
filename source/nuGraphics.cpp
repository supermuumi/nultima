#include "nuGraphics.h"
#include "nuDefs.h"
#include "nuLight.h"

#if NU_OS == NU_OS_WINDOWS
#   include <Windows.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "stb/stb_image.c"
#include <iostream>

using namespace Nultima;
using namespace std;

Graphics::Graphics(Vec2i windowDimensions) :
    m_windowDimensions(windowDimensions)
{
}

Graphics::~Graphics()
{
}

int Graphics::init()
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
	return 1;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    sdlWindow = SDL_CreateWindow("Nultima",
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 m_windowDimensions.m_x, m_windowDimensions.m_y,
                                 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (sdlWindow == NULL) {
	//std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	return 1;
    }
    
    sdlGLContext = SDL_GL_CreateContext(sdlWindow);
    SDL_GL_SetSwapInterval(1);

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdlRenderer == NULL) {
	//std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
	return 1;
    }

    // OpenGL state
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);

    return 0;
}

void Graphics::deinit()
{
    SDL_GL_DeleteContext(sdlGLContext);
    if (sdlRenderer != NULL)
        SDL_DestroyRenderer(sdlRenderer);
    if (sdlWindow != NULL)
        SDL_DestroyWindow(sdlWindow);
    SDL_Quit();    
}

void Graphics::resize(int x, int y)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    m_windowDimensions = Vec2i(x, y);

    glViewport(0, 0, x, y);

    setPerspectiveProjection();
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::setPerspectiveProjection()
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( 70.0, (GLfloat)(m_windowDimensions.m_x)/(GLfloat)(m_windowDimensions.m_y), 0.1f, 50000.0 );
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::setOrthoProjection(int left, int right, int bottom, int top)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(left, right, bottom, top);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::clear()
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glClearColor(0.1f, 0.2f, 0.4f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::swap()
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    SDL_GL_SwapWindow(sdlWindow);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::lookAt(Vec3 pos, Vec3 center, Vec3 up)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    gluLookAt(
        pos.m_x, pos.m_y, pos.m_z,
        center.m_x, center.m_y, center.m_z,
        up.m_x, up.m_y, up.m_z);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::translate(float x, float y, float z)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glTranslatef(x, y, z);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::translate(Vec3i v)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glTranslatef((float)v.m_x, v.m_y, v.m_z);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::scale(float x, float y, float z)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glScalef(x, y, z);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::rotate(float angle, float x, float y, float z)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glRotatef(angle, x, y, z);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::pushMatrix()
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glPushMatrix();
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::popMatrix()
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glPopMatrix();
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

unsigned int Graphics::createIndexBuffer(unsigned int* tris, int numTris)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    GLuint indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*numTris*3, tris, GL_STATIC_DRAW);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    return indexBufferObject;
}

void Graphics::bindIndexBuffer(unsigned int buffer)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)buffer);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

unsigned int Graphics::createVertexBuffer(float* verts, int numVerts)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    int vertexSize = sizeof(float)*8;

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, numVerts*vertexSize, verts, GL_STATIC_DRAW);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    return vertexBufferObject;
}

void Graphics::bindVertexBuffer(unsigned int buffer)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    int vertexSize = sizeof(float)*8;

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)buffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, vertexSize, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, vertexSize, (void*)(sizeof(float)*3));   //The starting point of normals, 12 bytes away
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, vertexSize, (void*)(sizeof(float)*6));
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::drawElements(int count)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glDrawElements(GL_TRIANGLES, count*3, GL_UNSIGNED_INT, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::bindTexture(unsigned int id)
{
    glBindTexture(GL_TEXTURE_2D, id);
}

unsigned int Graphics::loadTexture(std::string filename)
{
    int w, h, bpp;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &bpp, 0);

    GLuint ret;
    glGenTextures(1, &ret);
    glBindTexture( GL_TEXTURE_2D, ret );

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT );

    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data );

    free(data);

    return ret;
}

unsigned int Graphics::generateTexture(unsigned char* data, int w, int h)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    GLuint ret;
    glGenTextures(1, &ret);
    glBindTexture( GL_TEXTURE_2D, ret );

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT );

    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );
    NU_ASSERT(glGetError() == GL_NO_ERROR);

    return ret;
}

void Graphics::setTextureData(unsigned int id, unsigned char* data, int x, int y, int width, int height)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);
    glBindTexture( GL_TEXTURE_2D, id );
    glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        x,
                        y,
                        width,
                        height,
                        GL_RGBA,
                        GL_UNSIGNED_BYTE,
                        data);
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::setTexturing(bool enable)
{
    if (enable)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
}

void Graphics::setColor(float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
}

// TODO ripped from protos/editor/main.cpp - needs sanity check!
void Graphics::drawString(const char* str, float x, float y)
{
    NU_ASSERT(glGetError() == GL_NO_ERROR);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    int origY = m_windowDimensions.m_y-y;
    glRasterPos2i(x, origY);

    glDisable(GL_TEXTURE_2D);
    int i, len;
    for (i = 0, len = strlen(str); i < len; i++)
	{
		if (str[i] == '\n')
        {
			// TODO [muumi] get line height from somewhere
			origY -= 20;
			glRasterPos2i(x, origY);
		}
		else
		{
                    //TODO SDL_ttf?
                    //glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)str[i]);
		}
	}

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    NU_ASSERT(glGetError() == GL_NO_ERROR);
}

void Graphics::setBlending(bool enabled)
{
    if (enabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void Graphics::setDepthTest(bool enabled)
{
    if (enabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void Graphics::fillRect(float x1, float y1, float x2, float y2, bool blend)
{
    if (blend)
        setBlending(true);

    glMatrixMode(GL_MODELVIEW);
    pushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    popMatrix();

    if (blend)
        setBlending(false);
}

void Graphics::enableLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Graphics::disableLighting()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

// TODO refactoring needed, this is fugly
void Graphics::setLight(Light* light)
{
    float tmpVec[4];
    
    tmpVec[3] = 1.0f;
    // set color
    light->getAmbient(tmpVec);
    glLightfv(GL_LIGHT0, GL_AMBIENT, tmpVec);
    light->getDiffuse(tmpVec);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, tmpVec);
    light->getSpecular(tmpVec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, tmpVec);

    // set position
    Vec3 v = light->getPosition();
    v.getFloats(tmpVec);
    glLightfv(GL_LIGHT0, GL_POSITION, tmpVec);
}
