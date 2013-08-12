#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include "object3d.h"
#include "camera.h"
#include "vec3.h"

Camera::Camera() {
    pos = Vec3(.0f, .0f, .0f);
    dir = Vec3(.0f, .0f, -1.f);
    right = Vec3(1.f, .0f, .0f);
    up = Vec3(.0f, 1.f, 0.f);

    rotX = rotY = rotZ = .0f;
}

void Camera::setView() {
    Vec3 ViewPoint = pos+dir;
    gluLookAt(pos.x, pos.y, pos.z,
	      ViewPoint.x,ViewPoint.y,ViewPoint.z,
	      up.x, up.y, up.z);
}
