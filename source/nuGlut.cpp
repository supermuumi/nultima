#include "nuGlut.h"
#include "nuDefs.h"
#include "nuKeyboard.h"
#include "nuContext.h"
#include "nuGame.h"
#include "nuGraphics.h"

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK
#include "GL/glut.h"
#endif

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

static void specialKeyPressed(int key, int x, int y)
{
    NU_UNREF(key);
    NU_UNREF(x);
    NU_UNREF(y);

    //NU_ASSERT(!"Not implemented");
    Keyboard* keyboard = Context::get()->getKeyboard();
    if (key == GLUT_KEY_LEFT)
	keyboard->setKeyPressed(NU_KEY_LEFT, true);    
    if (key == GLUT_KEY_RIGHT)
	keyboard->setKeyPressed(NU_KEY_RIGHT, true);    
    if (key == GLUT_KEY_UP)
	keyboard->setKeyPressed(NU_KEY_UP, true);    
    if (key == GLUT_KEY_DOWN)
	keyboard->setKeyPressed(NU_KEY_DOWN, true);    
}

static void specialKeyReleased(int key, int x, int y)
{
    NU_UNREF(key);
    NU_UNREF(x);
    NU_UNREF(y);

    //NU_ASSERT(!"Not implemented");
    Keyboard* keyboard = Context::get()->getKeyboard();
    if (key == GLUT_KEY_LEFT)
	keyboard->setKeyPressed(NU_KEY_LEFT, false);    
    if (key == GLUT_KEY_RIGHT)
	keyboard->setKeyPressed(NU_KEY_RIGHT, false);    
    if (key == GLUT_KEY_UP)
	keyboard->setKeyPressed(NU_KEY_UP, false);    
    if (key == GLUT_KEY_DOWN)
	keyboard->setKeyPressed(NU_KEY_DOWN, false);    
}

static void click(int button, int updown, int x, int y)
{
    NU_UNREF(button);
    NU_UNREF(updown);
    NU_UNREF(x);
    NU_UNREF(y);
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
