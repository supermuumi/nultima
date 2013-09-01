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

    void resize(int x, int y);
    void clear();
    void swap();
    void setViewProjection();
    void lookAt(Vec3 pos, Vec3 center, Vec3 up);

private:
    int     m_wWidth;
    int     m_wHeight;
    GLUT*   m_GLUT;
};

}; // namespace
