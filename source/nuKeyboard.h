#pragma once

namespace Nultima
{

class Keyboard
{
public:
    Keyboard();
    ~Keyboard() {};

    void setKeyPressed(unsigned char key, bool down);

private:
    bool m_keysDown[256];
};

};
