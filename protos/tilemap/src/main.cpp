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

static int wWidth = 640;
static int wHeight = 480;
static float angle = .0f;

double timer = .0f;
double lastFrame = -1.f;
double timedelta;

Camera *cam;

void display(void) {
    timer = clock() / (CLOCKS_PER_SEC / 1000.0);
    timedelta = (lastFrame < .0) ? .0 : timer-lastFrame;
    lastFrame = timer;

    glClearColor(0.1f, 0.2f, 0.4f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    cam->setView();

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
#define CAM_SPEED 5.f
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
    gluPerspective( 45.0, (GLfloat)(wWidth)/(GLfloat)(wHeight), 0.1f, 50000.0 );
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

    // enable culling
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    cam = new Camera();
    cam->pos.z = 50.f;

    glutMainLoop();

    delete cam;

    return 0;
}
