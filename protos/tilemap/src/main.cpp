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

#define FRAND() ((float)rand() / RAND_MAX)

using namespace std;

void cleanup(void);       
void buildLevel(void);

static int wWidth = 800;
static int wHeight = 600;
static float angle = .0f;

double timer = .0f;
double lastFrame = -1.f;
double timedelta;

Camera *cam;

GLuint levelList;

GLuint textureRock;
GLuint textureWood;

void display(void) {
    timer = clock() / (CLOCKS_PER_SEC / 1000.0);
    timedelta = (lastFrame < .0) ? .0 : timer-lastFrame;
    lastFrame = timer;

    glClearColor(0.1f, 0.2f, 0.4f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    cam->setView();

    glCallList(levelList);
   
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

void buildLevel() {

#define NUM_LEVELS 3

    const char *level[NUM_LEVELS];
    level[0] = 
	"xxxxxxxx"
	"xoooooox"
	"xoooooox"
	"xoooooox"
	"xoooooox"
	"xoooooox"
	"xoooooox"
	"xxxxxxxx";

    level[1] = 
	"xxxxxxxx"
	"x______x"
	"x______x"
	"x______x"
	"x______x"
	"x______x"
	"x______x"
	"xxxxxxxx";

    level[2] = 
	"x______x"
	"________"
	"________"
	"________"
	"________"
	"________"
	"________"
	"x______x";

    levelList = glGenLists(1);
    glNewList(levelList, GL_COMPILE);

    for (int z = 0; z < NUM_LEVELS; z++) {
	for (int y = 0, ofs = 0; y < 8; y++) {
	    for (int x = 0; x < 8; x++, ofs++) {
		
		char ch = level[z][ofs];
		if (ch == '_') continue;
		if (ch == 'x') glBindTexture( GL_TEXTURE_2D, textureRock );
		if (ch == 'o') glBindTexture( GL_TEXTURE_2D, textureWood );
		
		glPushMatrix();
		glTranslatef((float)x, (float)y, (float)z);
		//glutWireCube(1.0);
		texturedCube(1.0);
		glPopMatrix();
	    }
	}
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

    cout << "Running OpenGL version: " << glGetString(GL_VERSION) << endl;
    cout << "Extensions:" << endl << glGetString(GL_EXTENSIONS) << endl;

    glEnable( GL_TEXTURE_2D );
    textureRock = loadTexture("rock.jpg");
    textureWood = loadTexture("wood.jpg");

    // enable culling
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    cam = new Camera();
    cam->pos.x = 3.5f;
    cam->pos.y = 3.5f;
    cam->pos.z = 8.5f;

    buildLevel();

    glutMainLoop();

    delete cam;

    return 0;
}
