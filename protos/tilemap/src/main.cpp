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

#include "world.h"
#include "gl_utils.h"
#include "camera.h"
#include "light.h"
#include "Cell.h"
#include "textures.h"
#include "Player.h"
#include "keyboard.h"
#include "inhabitant.h"
#include "Dice.h"

#define FRAND() ((float)rand() / RAND_MAX)

using namespace std;

void cleanup(void);	  
void buildWorld(void);
void movePlayer(int x, int y);
void handleKeys();

static int wWidth = 800;
static int wHeight = 600;
static float angle = .0f;

double timer = 0.f;
double fps = 12.f;
double timerDelta = 0.f;
double lastFrame = 0.f;

Camera *g_cam;
Light *g_sun;
Player *g_player;

bool g_inventoryVisible;

TextureManager *textures;

std::vector<Cell*> cells; //[WORLD_SIZE];
std::vector<Inhabitant*> inhabitants;

static void drawStats()
{
    char string[128];
    sprintf(string, "fps: %4.0f (%4.1f ms)", 1000/fps, timerDelta);
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
    
    glRasterPos2i(20, wHeight-20);
    
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
    for (int i = 0; i < WORLD_SIZE; i++) {
	if (g_cam->cubeInFrustum(cells[i]->x+CELL_SIZE/2.0f, cells[i]->y+CELL_SIZE/2.0f, 0.0f, CELL_SIZE/2.0f)) {
	    cells[i]->render();
	}
    }

    // Render inhabitants
    // TODO [sampo] culling
    for (std::vector<Inhabitant*>::iterator it = inhabitants.begin(); it != inhabitants.end(); ++it)
        (*it)->render();

    // render player
    g_player->render();

    if (g_inventoryVisible)
	g_player->m_inventory->render(wWidth, wHeight);

    drawStats();
   
    glutSwapBuffers();
    glutPostRedisplay();
}

void idle(void) 
{
    glutPostRedisplay();
}

void nextTurn()
{
    // TODO
}

void keyPressed(unsigned char key, int x, int y) 
{
    // always update the buffer just in case
    setKeyPressed(key, true);

    bool advanceTurn = false;

    if (key == 'i') 
    {
	g_inventoryVisible = !g_inventoryVisible;
    }

    if (advanceTurn)
	nextTurn();
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

    if (isKeyDown('y')) g_cam->rotateZ(dist);
    if (isKeyDown('h')) g_cam->rotateZ(-dist);
    if (isKeyDown('g')) g_cam->rotateX(-dist);
    if (isKeyDown('j')) g_cam->rotateX(dist);
    if (isKeyDown('t')) g_cam->rotateY(-dist);
    if (isKeyDown('b')) g_cam->rotateY(dist);
}

void specialKeyPressed(int key, int x, int y) 
{
    if (key == GLUT_KEY_LEFT) movePlayer(-1, 0);
    if (key == GLUT_KEY_RIGHT) movePlayer(1, 0);
    if (key == GLUT_KEY_UP) movePlayer(0, 1);
    if (key == GLUT_KEY_DOWN) movePlayer(0, -1); 
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

void buildWorld() 
{
    for (int i = 0; i < WORLD_SIZE; i++) 
    {
	Cell *c = new Cell(2);
	int x = i % WORLD_WIDTH;
	int y = i / WORLD_WIDTH;
	c->move((float)x*CELL_SIZE, (float)y*CELL_SIZE);
	c->load("worldmap.png", 0);
	c->load("worldmap2.png", 1);
	cells.push_back(c);
    }
}

extern void             stbi_image_free (void *retval_from_stbi_load);
extern unsigned char    *stbi_load      (char const *filename,     int *x, int *y, int *comp, int req_comp);

void initializeInhabitants()
{
    int mapWidth, mapHeight, bpp;    
    unsigned char* data = stbi_load("inhabitants.png", &mapWidth, &mapHeight, &bpp, 3);
    assert(mapWidth==CELL_SIZE && mapHeight==CELL_SIZE);

    for (int i=0; i<mapWidth*mapHeight; i++)
    {
        int ofs = i * 3;
        int r = data[ofs++];
        int g = data[ofs++];
        int b = data[ofs++];
        unsigned int c = (r << 16) | (g << 8) | b;
        if (c != 0x00808080)
            continue;
        
        Inhabitant* jope = new Inhabitant(0, 0, i % CELL_SIZE, i / CELL_SIZE);
        inhabitants.push_back(jope);
    }
    stbi_image_free(data);
}

void loadTextures() 
{
    textures = new TextureManager();
    textures->addTexture("rock", "rock.jpg");
    textures->addTexture("wood", "wood.jpg");
    textures->addTexture("grass", "grass.jpg");
    textures->addTexture("water", "water.gif");
}


bool playerCanMoveTo(int x, int y, int layer)
{
    int     cellId  = y/CELL_SIZE*WORLD_WIDTH + x/CELL_SIZE;
    Cell*   c       = cells.at(cellId);

    int block       = c->getBlockAt(x % CELL_SIZE, y % CELL_SIZE, g_player->layer);
    int below       = c->getBlockAt(x % CELL_SIZE, y % CELL_SIZE, g_player->layer-1);

    if (block == BLOCK_ROCK || block == BLOCK_WATER) 
    	return false;

    if (below == BLOCK_WATER)
        return false;

    // loop the inhabitants
    for (std::vector<Inhabitant*>::iterator it = inhabitants.begin(); it != inhabitants.end(); ++it)
    {
        int jopeCellX = (*it)->getCellX();
        int jopeCellY = (*it)->getCellY();

        if (cellId != jopeCellY*WORLD_SIZE + jopeCellX)
            continue;

        int jopeX = (*it)->getX();
        int jopeY = (*it)->getY();

        if (jopeX == x % CELL_SIZE && jopeY == y % CELL_SIZE)
            return false;
    }

    return true;
}

void movePlayer(int x, int y) 
{
    int newX = g_player->x + x;
    int newY = g_player->y + y;

    // wrap around
    if (newX < 0) newX = CELL_SIZE*WORLD_WIDTH-1;
    if (newY < 0) newY = CELL_SIZE*WORLD_WIDTH-1;
    if (newX >= CELL_SIZE*WORLD_WIDTH) newX = 0;
    if (newY >= CELL_SIZE*WORLD_WIDTH) newY = 0;

    if (playerCanMoveTo(newX, newY, g_player->layer))
        g_player->setPosition(newX, newY);
}

void initPlayer() 
{
    g_player = new Player();
}

void deinitPlayer() 
{
    delete g_player;
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
    g_cam->pos.x = CELL_SIZE / 2.0f; //3.5f;
    g_cam->pos.y = CELL_SIZE / 2.0f; //3.5f;
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
    buildWorld();
    initializeInhabitants();

    initPlayer();
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
    deinitPlayer();

    while (cells.size())
    {
	Cell* c = cells.back();
	delete c;
	cells.pop_back();
    }

    while (inhabitants.size())
    {
        Inhabitant* jope = inhabitants.back();
        delete jope;
        inhabitants.pop_back();
    }
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
