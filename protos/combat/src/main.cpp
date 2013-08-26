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
#include "Cell.h"
#include "textures.h"
#include "Player.h"
#include "monster.h"
#include "context.h"

#define FRAND() ((float)rand() / RAND_MAX)

using namespace std;

void cleanup(void);       
void buildWorld(void);
bool movePlayer(int x, int y);

static int wWidth = 800;
static int wHeight = 600;
static float angle = .0f;

double timer = 0.f;
double fps = 12.f;
double timerDelta = 0.f;
double lastFrame = 0.f;

Camera *cam;
GLuint worldList;
int g_currentPlayer = 0;
int g_currentMonster = -1;

TextureManager *textures;

Cell* g_arena = NULL;

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
    static const double ratio = 0.98f;
    timer = clock() / (CLOCKS_PER_SEC / 1000.0);
    timerDelta = timer - lastFrame;
    lastFrame = timer;
    fps = timerDelta * (1.f-ratio) + fps * ratio;

    if (g_currentMonster >= 0)
    {
        std::vector<Monster*> monsters = CONTEXT()->getMonsters();
        monsters[g_currentMonster]->tick();
        if (!monsters[g_currentMonster]->isActive())
        {
            g_currentMonster++;
            if (g_currentMonster == monsters.size())
            {
                // Player's turn
                g_currentMonster = -1;
                g_currentPlayer = 0;
            }
            else
            {
                std::vector<Monster*> monsters = CONTEXT()->getMonsters();
                monsters[g_currentMonster]->prepare();
            }
        }
    }

    setCameraMatrices();

    glColor4f(1, 1, 1, 1);
    glClearColor(0.1f, 0.2f, 0.4f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    cam->setView();

    // render world
	g_arena->render();

    // render heroes
    std::vector<Player*> heroes = CONTEXT()->getHeroes();
    for (std::vector<Player*>::iterator it = heroes.begin(); it != heroes.end(); ++it)
        (*it)->render(it-heroes.begin() == g_currentPlayer);

    // render monsters
    std::vector<Monster*> monsters = CONTEXT()->getMonsters();
    for (std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)
        (*it)->render(it-monsters.begin() == g_currentMonster);

    drawStats();
   
    glutSwapBuffers();
    glutPostRedisplay();
}

void idle(void) {
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    x; y;

    float dist = timerDelta * 0.1f;

    switch(key) {
    case 9: // tab
	break; 
    case 27: // esc
        exit (0);

    case 'a': cam->moveX(dist); break;
    case 'd': cam->moveX(-dist); break;
    case 'w': cam->moveY(dist); break;
    case 's': cam->moveY(-dist); break;
    case 'q': cam->moveZ(-dist); break;
    case 'z': cam->moveZ(dist); break;

    case 'y': cam->rotateZ(dist); break;
    case 'h': cam->rotateZ(-dist); break;
    case 'g': cam->rotateX(-dist); break;
    case 'j': cam->rotateX(dist); break;
    case 't': cam->rotateY(-dist); break;
    case 'b': cam->rotateY(dist); break;

    default: 
     cout << "pressed " << ((int)key) << endl;
        break;
    }
}

void specialKeys(int key, int x, int y)
{
    if (g_currentPlayer == -1)
        return;

    bool endTurn = false;
    int deltaX = 0;
    int deltaY = 0;

    if (key == GLUT_KEY_LEFT)
        deltaX = -1;
    else if (key == GLUT_KEY_RIGHT)
        deltaX = 1;
    else if (key == GLUT_KEY_UP)
        deltaY = 1;
    else if (key == GLUT_KEY_DOWN)
        deltaY = -1;

    if (movePlayer(deltaX, deltaY))
        g_currentPlayer++;

    if (g_currentPlayer == CONTEXT()->getHeroes().size())
    {
        // Monsters turn
        g_currentPlayer = -1;
        g_currentMonster = 0;
        std::vector<Monster*> monsters = CONTEXT()->getMonsters();
        monsters[g_currentMonster]->prepare();
    }
}

void click(int button, int updown, int x, int y)  {
    button; updown; x; y;
}

void motion (int x, int y)  {
    x; y;
    glutPostRedisplay();
}

void reshape(int x, int y) {
    wWidth = x; 
    wHeight = y;

    glViewport(0, 0, x, y);
    setCameraMatrices(); 
    glutPostRedisplay();
}

void cleanup(void)
{
}   

void buildWorld() {
	g_arena = new Cell(1);
	g_arena->move((float)0, (float)0);
	g_arena->load(0);
}

void loadTextures()
{
    textures = new TextureManager();
    textures->addTexture("grass", "grass.jpg");
}

bool movePlayer(int x, int y)
{
    int newX = CONTEXT()->getHeroes()[g_currentPlayer]->m_x + x;
    int newY = CONTEXT()->getHeroes()[g_currentPlayer]->m_y + y;

    // clip
    if (newX < 0 || newY < 0 || newX >= CELL_SIZE || newY >= CELL_SIZE) 
    	return false;

    // collide with other players
    std::vector<Player*> heroes = CONTEXT()->getHeroes();
    for (std::vector<Player*>::iterator it = heroes.begin(); it != heroes.end(); ++it)
    {
        if (it-heroes.begin() == g_currentPlayer)
            continue;

        if (newX == (*it)->m_x && newY == (*it)->m_y)
            return false;
    }

    heroes[g_currentPlayer]->setPosition(newX, newY);
    return true;
}

// TODO fix memleaks :-)
int main(int argc, char** argv) {

    srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(wWidth, wHeight);
    int win = glutCreateWindow("combat proto");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(click);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    //glutIdleFunc(idle);

    atexit(cleanup);

    //cout << "Running OpenGL version: " << glGetString(GL_VERSION) << endl;
    //cout << "Extensions:" << endl << glGetString(GL_EXTENSIONS) << endl;

    // textures
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    loadTextures();

    buildWorld();

    // enable culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    cam = new Camera();
    cam->pos.x = CELL_SIZE / 2.0f; //3.5f;
    cam->pos.y = CELL_SIZE / 2.0f; //3.5f;
    cam->pos.z = 15.0f;

    CONTEXT()->pushHero(new Player(5, 5));
    CONTEXT()->pushHero(new Player(7, 5));
    CONTEXT()->pushHero(new Player(9, 5));

    CONTEXT()->pushMonster(new Monster(4, 11));
    CONTEXT()->pushMonster(new Monster(4, 10));

    g_currentPlayer = 0;

    glutMainLoop();

    Context::cleanup();
    delete cam;
    return 0;
}
