#pragma once

#include <list>

namespace Nultima
{

#define NU_KEY_TAB          9
#define NU_KEY_LEFT         356
#define NU_KEY_UP           357
#define NU_KEY_RIGHT        358
#define NU_KEY_DOWN         359
#define NU_KEY_PAGE_UP      360
#define NU_KEY_PAGE_DOWN    361

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
