#pragma once

namespace Nultima
{

class Game;
class Graphics;

class GLUT
{
public:
    GLUT() {};
    ~GLUT() {};

    void init(int argc, char** argv, int wWidth, int wHeight);
    void deinit();

    static void mainloop(Game* game, Graphics* graphics);

    void swap();

private:

};

}; // namespace
