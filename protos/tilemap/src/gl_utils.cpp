#include <math.h>
#include "gl_utils.h"
#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

// loads .. a texture *gasp*
GLuint loadTexture(char* fname) {
    int w, h, bpp;
    unsigned char *data = stbi_load(fname, &w, &h, &bpp, 0);

    GLuint ret;
    glGenTextures(1, &ret);
    glBindTexture( GL_TEXTURE_2D, ret );

    printf("loadTexture(%s): ret=%d\n", fname, ret);

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

void cubeAt(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidCube(1.0);
    glPopMatrix();
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

void texturedPlane(GLfloat size) {
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-size/2.0f, -size/2.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(size/2.0f, -size/2.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(size/2.0f, size/2.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-size/2.0f, size/2.0f, 0.0f);
    glEnd();
}
