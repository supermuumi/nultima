#pragma once

#include <string>

namespace Nultima
{

class Graphics;
class Keyboard;

class Context
{
public:
    Context();
    ~Context();

    static Context* get();
    void            release();

    void            init(int argc, char** argv);
    void            deinit();

    std::string     getWorldFile();
    std::string     getStateFile();
    Graphics*       getGraphics()   { return m_graphics; }
    Keyboard*       getKeyboard()   { return m_keyboard; }

private:
    int         m_argc;
    char**      m_argv;
    Graphics*   m_graphics;
    Keyboard*   m_keyboard;
};

}; // namespace
