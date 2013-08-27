#include <stdio.h>
#include "keyboard.h"

bool* keysDown;

void initKeyboard() {
    keysDown = new bool[256];
    for (int i = 0; i < 256; i++) 
	keysDown[i] = false;
}

void deinitKeyboard() {
    delete keysDown;
}

void setKeyPressed(unsigned char key, bool down) {
    keysDown[key] = down;
}

bool isKeyDown(unsigned char key) {
    return keysDown[key];
}

bool isKeyUp(unsigned char key) {
    return !keysDown[key];
}

