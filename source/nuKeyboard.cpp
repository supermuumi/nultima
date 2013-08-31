#include "nuKeyboard.h"

using namespace Nultima;

Keyboard::Keyboard()
{
    for (int i = 0; i < 256; i++) 
        m_keysDown[i] = false;
}

void Keyboard::setKeyPressed(unsigned char key, bool down)
{
    m_keysDown[key] = down;
}
