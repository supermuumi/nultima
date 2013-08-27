#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KEY_UP 255
#define KEY_DOWN 254
#define KEY_LEFT 253
#define KEY_RIGHT 252

void initKeyboard();
void deinitKeyboard();
void setKeyPressed(unsigned char key, bool down);
bool isKeyDown(unsigned char key);
bool isKeyUp(unsigned char key);

//extern bool* keyDownStates;

#endif
