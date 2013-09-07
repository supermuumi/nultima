#pragma once

#include "nuVec3.h"
#include <string>

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
    unsigned int createIndexBuffer  (unsigned int* tris, int numTris);
    void         bindIndexBuffer    (unsigned int buffer);
    unsigned int createVertexBuffer (float* verts, int numVerts);
    void         bindVertexBuffer   (unsigned int buffer);
    void         drawElements       (int count);

    // Texturing
    void            bindTexture (unsigned int id);
    unsigned int    loadTexture (std::string filename);

    // Render target ops
    void resize (int x, int y);
    void clear  ();
    void swap   ();

    // Matrix ops
    void loadIdentity       ();
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
