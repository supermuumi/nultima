#pragma once

#include "nuVec3.h"

namespace Nultima
{

class GLUT;

class Graphics
{
public:
    Graphics(int wWidth, int wHeight);
    ~Graphics();

    void init(int argc, char** argv);
    void deinit();

    // Geometry
    void*   createIndexBuffer   (unsigned int* tris, int numTris);
    void    bindIndexBuffer     (void* buffer);
    void*   createVertexBuffer  (float* verts, int numVerts);
    void    bindVertexBuffer    (void* buffer);
    void    drawElements        (int count);

    // Render target ops
    void resize (int x, int y);
    void clear  ();
    void swap   ();

    // Matrix ops
    void setViewProjection  ();
    void lookAt             (Vec3 pos, Vec3 center, Vec3 up);
    void pushMatrix         ();
    void popMatrix          ();
    void translate          (float x, float y, float z);

private:
    int     m_wWidth;
    int     m_wHeight;
    GLUT*   m_GLUT;
};

}; // namespace
