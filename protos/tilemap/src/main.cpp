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
#include "Cell.h"
#include "textures.h"

#define FRAND() ((float)rand() / RAND_MAX)

using namespace std;

void cleanup(void);       
void buildWorld(void);

static int wWidth = 800;
static int wHeight = 600;
static float angle = .0f;

double timer = .0f;
double lastFrame = -1.f;
double timedelta;

Camera *cam;
Light *light;
GLuint worldList;

TextureManager *textures;
#define WORLD_WIDTH 4
#define WORLD_SIZE (WORLD_WIDTH*WORLD_WIDTH)

Cell cells[WORLD_SIZE];

void display(void) {
    timer = clock() / (CLOCKS_PER_SEC / 1000.0);
    timedelta = (lastFrame < .0) ? .0 : timer-lastFrame;
    lastFrame = timer;

    glClearColor(0.1f, 0.2f, 0.4f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // lighting
    float poo[4];
    light->ambient.getFloats(poo);
    glLightfv( GL_LIGHT0, GL_AMBIENT, poo);
    light->diffuse.getFloats(poo);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, poo);
    light->specular.getFloats(poo);
    glLightfv( GL_LIGHT0, GL_SPECULAR, poo);

    float position[] = { cos(timer*0.0005f)*12.f, sin(timer*.0005f)*25.f, sin(timer*0.0005f)*14.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glShadeModel(GL_SMOOTH);

    glLoadIdentity();
    cam->setView();

    // TODO frustum culling
    for (int i = 0; i < WORLD_SIZE; i++)
	cells[i].render();
   
    glutSwapBuffers();
    glutPostRedisplay();
}

void idle(void) {
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    x; y;

    switch(key) {
    case 9: // tab
	break; 
    case 27: // esc
        exit (0);

	// TODO fix frame rate dependency
#define CAM_SPEED 10.5f
    case 'w': cam->moveZ(CAM_SPEED); break;
    case 's': cam->moveZ(-CAM_SPEED); break;
    case 'a': cam->moveX(-CAM_SPEED); break;
    case 'd': cam->moveX(CAM_SPEED); break;
    case 'q': cam->moveY(-CAM_SPEED); break;
    case 'z': cam->moveY(CAM_SPEED); break;

    case 'y': cam->rotateZ(CAM_SPEED); break;
    case 'h': cam->rotateZ(-CAM_SPEED); break;
    case 'g': cam->rotateX(-CAM_SPEED); break;
    case 'j': cam->rotateX(CAM_SPEED); break;
    case 't': cam->rotateY(-CAM_SPEED); break;
    case 'b': cam->rotateY(CAM_SPEED); break;

 default: 
     cout << "pressed " << ((int)key) << endl;
        break;
    }

printf("cam pos = %.2f %.2f %.2f\n", cam->pos.x, cam->pos.y, cam->pos.z);
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0, 1, 1, 0, 0, 1); 
    gluPerspective( 60.0, (GLfloat)(wWidth)/(GLfloat)(wHeight), 0.1f, 50000.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

void cleanup(void) {
    // TODO
}


void buildWorld() {
    for (int i = 0; i < WORLD_SIZE; i++) {
	int x = i % WORLD_WIDTH;
	int y = i / WORLD_WIDTH;
	cells[i].move((float)x*CELL_SIZE, (float)y*CELL_SIZE);
	cells[i].load("worldmap.png");
    }
}

void loadTextures() {
    textures = new TextureManager();
    textures->addTexture("rock", "rock.jpg");
    textures->addTexture("wood", "wood.jpg");
    textures->addTexture("grass", "grass.jpg");
    textures->addTexture("water", "water.gif");
}


int main(int argc, char** argv) {

    srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(wWidth, wHeight);
    int win = glutCreateWindow("tilemap proto");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(click);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    //glutIdleFunc(idle);

    atexit(cleanup);

    //cout << "Running OpenGL version: " << glGetString(GL_VERSION) << endl;
    //cout << "Extensions:" << endl << glGetString(GL_EXTENSIONS) << endl;

    // set up lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Create light components
    float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
    float specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };


    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);


    //glShadeModel(GL_SMOOTH);

    float As[4] = {0.1f, 0.1f, 0.1f, 1.0f };
    float Al[4] = {0.0f, 0.0f, 0.0f, 1.0f };
    float Dl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
    float Sl[4] = {1.0f, 1.0f, 1.0f, 1.0f };
    float Am[4] = {0.3f, 0.3f, 0.3f, 1.0f };
    float Dm[4] = {0.9f, 0.5f, 0.5f, 1.0f };
    float Sm[4] = {0.6f, 0.6f, 0.6f, 1.0f };
    float f = 60.0f;

    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, As );

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Am );
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dm );
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Sm );
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, f );

    // textures
    glEnable( GL_TEXTURE_2D );
    loadTextures();


    buildWorld();

    // enable culling
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    cam = new Camera();
    cam->pos.x = 3.5f;
    cam->pos.y = 3.5f;
    cam->pos.z = 8.5f;

    light = new Light();

    //buildLevel();
    //buildWorld();

    glutMainLoop();

    delete cam;

    return 0;
}
