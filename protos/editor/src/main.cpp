#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "stb_image.c"

#include "gl_utils.h"
#include "camera.h"
#include "light.h"
#include "textures.h"
#include "keyboard.h"
#include "Cell.h"
#include "World.h"

#define FRAND() ((float)rand() / RAND_MAX)

using namespace std;

void cleanup(void);	  
void movePlayer(int x, int y);
void handleKeys();

static int wWidth = 800;
static int wHeight = 600;
static float angle = .0f;

double timer = 0.f;
double fps = 12.f;
double timerDelta = 0.f;
double lastFrame = 0.f;

// editor stuff
int activeCellX;
int activeCellY;
int activeBlock;
int activeLayer;
bool paintMode;

World *g_world;
Camera *g_cam;
Light *g_sun;

bool g_inventoryVisible;

TextureManager *textures;

//std::vector<Cell*> cells; //[WORLD_SIZE];

static void drawString(char *string, int x, int y)
{
/*
    char string[128];
    sprintf(string, "fps: %4.0f (%4.1f ms) (%.4f) / cam=[%.2f,%.2f,%.2f]", 1000/fps, timerDelta, g_cam->pos.x, g_cam->pos.y, g_cam->pos.z);

*/
    int i, len; 
   
    glColor4f(1, 1, 1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0.0, wWidth, 0.0, wHeight);
    //glTranslatef(0.0f, 0.0f, -5.0f);
    
    glRasterPos2i(x, y); //20, wHeight-20);
    
    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    for (i = 0, len = strlen(string); i < len; i++)
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)string[i]);

    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void setCameraMatrices()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0, 1, 1, 0, 0, 1); 
    gluPerspective( 70.0, (GLfloat)(wWidth)/(GLfloat)(wHeight), 0.1f, 50000.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void) 
{
    handleKeys();

    static const double ratio = 0.98f;
    timer = clock() / (CLOCKS_PER_SEC / 1000.0);
    timerDelta = timer - lastFrame;
    lastFrame = timer;
    fps = timerDelta * (1.f-ratio) + fps * ratio;

    setCameraMatrices();

    glColor4f(1, 1, 1, 1);
    glClearColor(0.1f, 0.2f, 0.4f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    g_cam->setView();

    // render world
    g_world->render(g_cam);

    // draw cursor
    glPushMatrix();
    glTranslatef(activeCellX, -activeCellY, 0.0f);
    int tempTimer = (int)timer;
    if (tempTimer % 600 < 300 || activeBlock == BT_BLANK)
    {
	glDisable(GL_TEXTURE);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireCube(1.0);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);
    }
    else
    {
// TODO fix proper
/*
	glBindTexture(GL_TEXTURE_2D, cells[0]->getTextureForBlock(activeBlock));
	texturedCube(1.0);
*/
    }


    glPopMatrix();

    char string[128];
    sprintf(string, "fps: %4.0f (%4.1f ms) (%.4f) / cam=[%.2f,%.2f,%.2f]", 1000/fps, timerDelta, g_cam->pos.x, g_cam->pos.y, g_cam->pos.z);
    drawString(string, 20, wHeight-20);
    char *helptext = "p=paint on/off   o=paint current   kl=prev/next layer   .,=prev/next block   arrows=move";
    drawString(helptext, 20, wHeight-34);
   
    glutSwapBuffers();
    glutPostRedisplay();
}

void idle(void) 
{
    glutPostRedisplay();
}

void paintCurrentBlock()
{
/*
    int     cellId  = activeCellY/CELL_SIZE*WORLD_WIDTH + activeCellX/CELL_SIZE;
    Cell*   c       = cells.at(cellId);
*/
    g_world->setBlockAt(activeCellX, activeCellY, activeLayer, activeBlock);
}


void eraseCurrentBlock()
{
    g_world->setBlockAt(activeCellX, activeCellY, activeLayer, BT_BLANK);
}



void keyPressed(unsigned char key, int x, int y) 
{
    // always update the buffer just in case
    setKeyPressed(key, true);

    if (key == 'p') 
    {
	paintMode = !paintMode;
    }
    if (key == 'o')
    {
	paintCurrentBlock();
    }
    if (key == 'i') {
	eraseCurrentBlock();
    }
    if (key == '.') 
    {
	activeBlock++;
	if (activeBlock == BT_LASTBLOCK)
	    activeBlock = BT_BLANK+1;
    }
    if (key == ',') 
    {
	activeBlock--;
	if (activeBlock < 0)
	    activeBlock = BT_LASTBLOCK-1;
    }

    if (key == 'k' && activeLayer > 0) activeLayer--;
    if (key == 'l' && activeLayer < 1) activeLayer++;
}

void keyReleased(unsigned char key, int x, int y) 
{
    setKeyPressed(key, false);
}

void handleKeys() 
{
    float dist = timerDelta * 0.03f;

    if (isKeyDown('a')) g_cam->moveX(dist); 
    if (isKeyDown('d')) g_cam->moveX(-dist); 
    if (isKeyDown('w')) g_cam->moveY(dist); 
    if (isKeyDown('s')) g_cam->moveY(-dist); 
    if (isKeyDown('q')) g_cam->moveZ(-dist); 
    if (isKeyDown('z')) g_cam->moveZ(dist); 
}

void specialKeyPressed(int key, int x, int y) 
{
    if (key == GLUT_KEY_LEFT && activeCellX > 0) activeCellX--; 
    if (key == GLUT_KEY_RIGHT && activeCellX < g_world->getWidth()-1) activeCellX++;
    if (key == GLUT_KEY_UP && activeCellY > 0) activeCellY--; 
    if (key == GLUT_KEY_DOWN && activeCellY < g_world->getHeight()-1) activeCellY++;

    if (paintMode) 
	paintCurrentBlock();

    g_cam->pos.x = activeCellX;
    g_cam->pos.y = -activeCellY;
}

void specialKeyReleased(int key, int x, int y) 
{
}

void click(int button, int updown, int x, int y)  
{
    button; updown; x; y;
}

void motion (int x, int y)  
{
    x; y;
    glutPostRedisplay();
}

void reshape(int x, int y) 
{
    wWidth = x; 
    wHeight = y;

    glViewport(0, 0, x, y);
    setCameraMatrices(); 
    glutPostRedisplay();
}

void cleanup(void) 
{
    // TODO
}

void initWorld() 
{
    activeCellX = activeCellY = 0;
    activeBlock = BT_BLANK+1;
    paintMode = false;

    g_world = new World(4, 4, 16);
}

void deinitWorld()
{
    delete g_world;
}

extern void             stbi_image_free (void *retval_from_stbi_load);
extern unsigned char    *stbi_load      (char const *filename,     int *x, int *y, int *comp, int req_comp);

void loadTextures() 
{
    textures = new TextureManager();
    textures->addTexture("rock", "rock.jpg");
    textures->addTexture("wood", "wood.jpg");
    textures->addTexture("grass", "grass.jpg");
    textures->addTexture("water", "water.gif");
}

void initLighting() 
{
    g_sun = new Light();
}

void deinitLighting() 
{
    delete g_sun;
}

void initCamera() 
{
    g_cam = new Camera();
    g_cam->pos.x = 0.0f;
    g_cam->pos.y = 0.0f;
    g_cam->pos.z = 10.0f;
}

void deinitCamera() 
{
    delete g_cam;
}

void setupOpenGL() 
{
    // textures
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    loadTextures();

    // enable culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void initGame()
{
    initWorld();

    initCamera();
    initLighting();
    initKeyboard();

    g_inventoryVisible = false;
}

void deinitGame() 
{
    deinitKeyboard();
    deinitLighting();
    deinitCamera();

    deinitWorld();
}

// TODO fix memleaks :-)
int main(int argc, char** argv) 
{

    srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(wWidth, wHeight);
    int win = glutCreateWindow("tilemap proto");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);
    glutSpecialFunc(specialKeyPressed);
    glutSpecialUpFunc(specialKeyReleased);
    glutMouseFunc(click);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    //glutIdleFunc(idle);

    atexit(cleanup);

    //cout << "Running OpenGL version: " << glGetString(GL_VERSION) << endl;
    //cout << "Extensions:" << endl << glGetString(GL_EXTENSIONS) << endl;

    setupOpenGL();
    initGame();

    glutMainLoop();

    deinitGame();

    return 0;
}
