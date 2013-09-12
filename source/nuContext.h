#pragma once

#include <string>
#include <vector>
#include "nuTilemap.h"

namespace Nultima
{

class Graphics;
class Keyboard;
class Model;
class TexManager;
class Mouse;

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
    Graphics*       getGraphics ()                  { return m_graphics; }
    Keyboard*       getKeyboard ()                  { return m_keyboard; }
    Mouse*          getMouse    ()                  { return m_mouse; }
    const Model*    getModel    (int type);
    unsigned int    getTexture  (std::string name);

    TexManager*     getTexManager();
    unsigned int    getTilemapTexture(int id);

    int             getArgc() { return m_argc; }
    char**          getArgv() { return m_argv; }
    bool            isFullScreen() { return m_fullScreen; }

private:
    void            parseCommandLine();

    int                 m_argc;
    char**              m_argv;
    Graphics*           m_graphics;
    Keyboard*           m_keyboard;
    Mouse*              m_mouse;
    std::vector<Model*> m_models;
    TexManager*         m_texManager;
    bool                m_fullScreen;
};

}; // namespace
