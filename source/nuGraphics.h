#pragma once

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

private:
    int     m_wWidth;
    int     m_wHeight;
    GLUT*   m_GLUT;
};

}; // namespace
