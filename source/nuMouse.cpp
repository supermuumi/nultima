#include "nuMouse.h"

using namespace Nultima;

Mouse::Mouse()
{
    for (int i=0; i<2; i++)
    {
        m_keysDown[i].isDown = false;
        m_keysDown[i].x = 0;
        m_keysDown[i].y = 0;
    }
}

void Mouse::setClick(int button, int x, int y, bool isDown)
{
    m_keysDown[button].isDown = isDown;
    m_keysDown[button].x = x;
    m_keysDown[button].y = y;
}

