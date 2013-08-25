#include "Player.h"
#include "Cell.h"
#include "gl_utils.h"

#include <time.h>
#include <stdio.h>
#include <assert.h>

static const Vec3 s_defaultColor = Vec3(0.8, 0.8, 0.5);
static const Vec3 s_blinkColor = Vec3(0.6, 0.6, 0.6);

Player::Player(int x, int y) :
    m_x(x),
    m_y(y),
    m_lastColorChange(0),
    m_color(s_defaultColor)
{}

void Player::setPosition(int _x, int _y)
{
    m_x = _x;
    m_y = _y;
}

void Player::render(bool isActive)
{
    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    double timer = clock() / (CLOCKS_PER_SEC / 1000.0);
    if (isActive && timer - m_lastColorChange >= 300.f)
    {
        m_lastColorChange = timer;
        m_color = (m_color == s_defaultColor) ? s_blinkColor : s_defaultColor;
    }

    if (!isActive)
        m_color = s_defaultColor;

    glPushMatrix();
    glColor3f(m_color.x, m_color.y, m_color.z);
    cubeAt((float)m_x, (float)m_y, 0.5f);
    glPopMatrix();

    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
}
