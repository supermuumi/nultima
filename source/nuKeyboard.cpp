#include "nuKeyboard.h"
#include <list>

using namespace Nultima;

Keyboard::Keyboard()
{
    for (int i = 0; i < 256; i++) 
        m_keysDown[i] = false;
}

void Keyboard::setKeyPressed(unsigned int key, bool down)
{
    m_keysDown[key] = down;
	if (down)
		m_keyPressBuffer.push_back(key);
}

bool Keyboard::isKeyPressed(unsigned int key) 
{
    return m_keysDown[key];
}

void Keyboard::resetKeypressBuffer()
{
	m_keyPressBuffer.clear();
}

bool Keyboard::hasKeyPresses()
{
	return !m_keyPressBuffer.empty();
}

int Keyboard::processKeyPress()
{
	int key = m_keyPressBuffer.front();
	m_keyPressBuffer.pop_front();
	return key; 
}

