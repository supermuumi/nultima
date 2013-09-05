#pragma once

namespace Nultima
{

#define NU_KEY_LEFT  256
#define NU_KEY_RIGHT 257
#define NU_KEY_UP    258
#define NU_KEY_DOWN  259

class Keyboard
{
public:
    Keyboard();
    ~Keyboard() {};

    void setKeyPressed(unsigned int key, bool down);
    bool isKeyPressed(unsigned int key);

private:
    bool m_keysDown[512];
};

};
