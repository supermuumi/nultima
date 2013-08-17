#ifndef __GL_UTILS_H_
#define __GL_UTILS_H_

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void cubeAt(GLfloat x, GLfloat y, GLfloat z);
void texturedCube(GLfloat size);

GLuint loadTexture(char *fname);

#endif
