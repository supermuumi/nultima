#include "nuGlut.h"
#include "nuDefs.h"
#include "nuKeyboard.h"
#include "nuContext.h"
#include "nuGame.h"
#include "nuGraphics.h"
#include "nuMouse.h"

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glut.h"
#endif

#define SPECIAL_KEY_OFFSET 256

using namespace Nultima;

Game* g_currentGame = NULL;
Graphics* g_graphics = NULL;

static void display()
{
    NU_ASSERT(g_currentGame);
    g_currentGame->tick();
    g_currentGame->display();
}

static void keyPressed(unsigned char key, int x, int y)
{
    NU_UNREF(x);
    NU_UNREF(y);

    Keyboard* keyboard = Context::get()->getKeyboard();
    keyboard->setKeyPressed(key, true);
}

static void keyReleased(unsigned char key, int x, int y)
{
    NU_UNREF(x);
    NU_UNREF(y);

    Keyboard* keyboard = Context::get()->getKeyboard();
    keyboard->setKeyPressed(key, false);
}

static void keyPress(int key, bool up)
{
    Keyboard* keyboard = Context::get()->getKeyboard();
    keyboard->setKeyPressed(key, up);
}

static void specialKeyPressed(int key, int x, int y)
{
    NU_UNREF(x);
    NU_UNREF(y);
    keyPress(key+SPECIAL_KEY_OFFSET, true);
}

static void specialKeyReleased(int key, int x, int y)
{
    NU_UNREF(x);
    NU_UNREF(y);
    keyPress(key+SPECIAL_KEY_OFFSET, false);
}

static void click(int button, int updown, int x, int y)
{
    Mouse* mouse = Context::get()->getMouse();
    mouse->setClick(button, x, glutGet(GLUT_WINDOW_HEIGHT)-y, (updown == GLUT_UP) ? false : true);
}

static void motion(int x, int y)
{
    NU_UNREF(x);
    NU_UNREF(y);

    glutPostRedisplay();
}

static void reshape(int x, int y)
{
    NU_ASSERT(g_graphics);
    g_graphics->resize(x, y);
    glutPostRedisplay();
}
static void cleanup()
{
}

void GLUT::init(int argc, char** argv, int wWidth, int wHeight)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(wWidth, wHeight);
    glutCreateWindow("Nultima");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);
    glutSpecialFunc(specialKeyPressed);
    glutSpecialUpFunc(specialKeyReleased);
    glutMouseFunc(click);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    atexit(cleanup);
}

void GLUT::deinit()
{
}

void GLUT::mainloop(Game* game, Graphics* graphics)
{
    g_currentGame = game;
    g_graphics = graphics;

    glutMainLoop();
}

void GLUT::swap()
{
    glutSwapBuffers();
    glutPostRedisplay();
}
