#include <stdio.h>
#include <assert.h>
#include "world.h"
#include "Cell.h"
#include "Player.h"
#include "gl_utils.h"

Player::Player() {

    m_strength = m_dexterity = m_intelligence = PLAYER_ATTR_DEFAULT;

    inventory = new Inventory();

    x = y = 0;
    layer = 1;
}

Player::~Player()
{
    delete inventory;
}

void Player::setPosition(int _x, int _y) {
    assert(x >= 0 && y >= 0);
    x = _x;
    y = _y;
}

void Player::render() {
    glPushMatrix();
    cubeAt((float)x, (float)y, layer-0.5f);
    glPopMatrix();
}
