#pragma once

#include <list>
#include <SDL2/SDL.h>

namespace Nultima
{

#define NU_KEY_TAB          SDLK_TAB
#define NU_KEY_LEFT         SDLK_LEFT
#define NU_KEY_UP           SDLK_UP
#define NU_KEY_RIGHT        SDLK_RIGHT
#define NU_KEY_DOWN         SDLK_DOWN
#define NU_KEY_PAGE_UP      SDLK_PAGEUP
#define NU_KEY_PAGE_DOWN    SDLK_PAGEDOWN
#define NU_KEY_HOME         SDLK_HOME
#define NU_KEY_END          SDLK_END
#define NU_KEY_RETURN       SDLK_RETURN
    
class Keyboard
{
public:
    Keyboard();
    ~Keyboard() {};

    void setKeyPressed(unsigned int key, bool down);
    bool isKeyPressed(unsigned int key);
    void resetKeypressBuffer();
    bool hasKeyPresses();
    int  processKeyPress();

private:
    bool m_keysDown[512];
    std::list<int> m_keyPressBuffer;
};

};
