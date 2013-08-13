#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "stb_image.c"
#include "camera.h"
#include "light.h"

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

GLuint textureRock;
GLuint textureWood;
GLuint textureGrass;
GLuint textureWater;

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

    glCallList(worldList);
   
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
#define CAM_SPEED .5f
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


GLuint loadTexture(char* fname) {
    int w, h, bpp;
    unsigned char *data = stbi_load(fname, &w, &h, &bpp, 0);

    GLuint ret;
    glGenTextures(1, &ret);
    glBindTexture( GL_TEXTURE_2D, ret );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		     GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, 
		       GL_RGB, GL_UNSIGNED_BYTE, data );

    return ret;
}

// drawBox() from GLUT sources
void texturedCube(GLfloat size) {
    static GLfloat n[6][3] = {
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] = {

	{0, 1, 2, 3},
	{3, 2, 6, 7},
	{7, 6, 5, 4},
	{4, 5, 1, 0},
	{5, 6, 2, 1},
	{7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
	glBegin(GL_QUADS);
	glNormal3fv(&n[i][0]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv(&v[faces[i][0]][0]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv(&v[faces[i][1]][0]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv(&v[faces[i][2]][0]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv(&v[faces[i][3]][0]);
	glEnd();
    }
}

void buildWorld() {
#define NUM_LAYERS 2
    int mapWidth, mapHeight, bpp;
    unsigned char *mapData[NUM_LAYERS];

    mapData[0] = stbi_load("worldmap.png", &mapWidth, &mapHeight, &bpp, 3);
    mapData[1] = stbi_load("worldmap2.png", &mapWidth, &mapHeight, &bpp, 3);
    
    worldList = glGenLists(1);
    glNewList(worldList, GL_COMPILE);
    
    for (int z = 0; z < NUM_LAYERS; z++) {
	int ofs = 0;
	for (int y = 0; y < mapHeight; y++) {
	    for (int x = 0; x < mapWidth; x++) {

		int r = mapData[z][ofs++];
		int g = mapData[z][ofs++];
		int b = mapData[z][ofs++];
		int p = (r << 16) | (g << 8) | b;
	    
		switch (p) {
		case 0x0000ff: glBindTexture(GL_TEXTURE_2D, textureWater); break;
		case 0x00ff00: glBindTexture(GL_TEXTURE_2D, textureGrass); break;
		case 0x808080: glBindTexture(GL_TEXTURE_2D, textureRock); break;
		case 0xff8000: glBindTexture(GL_TEXTURE_2D, textureWood); break;
		default: continue;
		}

		glPushMatrix();
		glTranslatef((float)x, (float)y, (float)z);
		texturedCube(1.0);
		glPopMatrix();
	    }
	}
	stbi_image_free(mapData[z]);
    }
    glEndList();



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
    textureRock = loadTexture("rock.jpg");
    textureWood = loadTexture("wood.jpg");
    textureGrass = loadTexture("grass.jpg");
    textureWater = loadTexture("water.gif");

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
    printf("start time = %.4f\n", clock() / (CLOCKS_PER_SEC / 1000.0));
    buildWorld();
    printf("end time = %.4f\n", clock() / (CLOCKS_PER_SEC / 1000.0));

    glutMainLoop();

    delete cam;

    return 0;
}
