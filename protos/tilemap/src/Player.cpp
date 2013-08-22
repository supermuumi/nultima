#include <stdio.h>
#include <assert.h>
#include "world.h"
#include "Cell.h"
#include "Player.h"
#include "gl_utils.h"

Player::Player() {
    x = y = 0;
    layer = 1;
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
