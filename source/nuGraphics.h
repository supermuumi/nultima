#pragma once

#include "nuVec3.h"
#include "nuVec2.h"

#include <string>

namespace Nultima
{

class GLUT;

class Graphics
{
public:
    Graphics(Vec2i windowDimensions);
    ~Graphics();

    void init(int argc, char** argv);
    void deinit();

    // Window
    Vec2i       getWindowDimensions () { return m_windowDimensions; }

    // Geometry
    unsigned int createIndexBuffer  (unsigned int* tris, int numTris);
    void         bindIndexBuffer    (unsigned int buffer);
    unsigned int createVertexBuffer (float* verts, int numVerts);
    void         bindVertexBuffer   (unsigned int buffer);
    void         drawElements       (int count);

    // Texturing
    void            bindTexture     (unsigned int id);
    unsigned int    loadTexture     (std::string filename);
    unsigned int    generateTexture (unsigned char* data, int w, int h);
    void            setTextureData  (unsigned int id, unsigned char* data, int x, int y, int width, int height);

    // Render target ops
    void resize (int x, int y);
    void clear  ();
    void swap   ();

    // Matrix ops
    void setPerspectiveProjection   ();
    void setOrthoProjection         (int left, int right, int bottom, int top);
    void lookAt                     (Vec3 pos, Vec3 center, Vec3 up);
    void pushMatrix                 ();
    void popMatrix                  ();
    void translate                  (float x, float y, float z);
    void scale                      (float x, float y, float z);

    // rendering
    void setColor(float r, float g, float b, float a);
    void drawString(const char* str, float x, float y);
    void fillRect(float x1, float y1, float x2, float y2, bool blend);
    void setBlending(bool enabled);
    void setDepthTest(bool enabled);

private:
    Vec2i   m_windowDimensions;
    GLUT*   m_GLUT;
};

}; // namespace
