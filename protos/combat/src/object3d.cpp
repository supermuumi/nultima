#define DEG2RAD 0.0174532925f

#include <math.h>
#include "object3d.h"

Object3D::Object3D() {
    pos = Vec3(.0f, .0f, .0f);
    dir = Vec3(.0f, .0f, -1.f);
    right = Vec3(1.f, .0f, .0f);
    up = Vec3(.0f, 1.f, 0.f);

    rotX = rotY = rotZ = .0f;
}

void Object3D::move(Vec3 dir) {
    pos += dir;
}

void Object3D::moveZ(float dist) {
    pos += dir*-dist;
}

void Object3D::moveX(float dist) {
    pos += right*-dist;
}

void Object3D::moveY(float dist) {
    pos += up*dist;
}

void Object3D::rotateX (float angle)
{
    rotX += angle;
	
    //rotate viewdir around the right vector:
    Vec3 v1 = dir * cos(angle*DEG2RAD);
    v1 += up * sin(angle*DEG2RAD);
    dir = v1.unit();

    //now compute the new UpVector (by cross product)
    up = dir.cross(right) * -1;
}

void Object3D::rotateY (float angle)
{
	rotY += angle;
	
	//rotate viewdir around the up vector:
	Vec3 v1 = dir*cos(angle*DEG2RAD);
	v1 -= right*sin(angle*DEG2RAD);
	dir = v1.unit();

	//now compute the new RightVector (by cross product)
	right = dir.cross(up);
}

void Object3D::rotateZ (float angle)
{
	rotZ += angle;
	
	//rotate viewdir around the right vector:
	Vec3 v1 = right*cos(angle*DEG2RAD);
	v1 += up*sin(angle*DEG2RAD);
	right = v1.unit();

	//now compute the new UpVector (by cross product)
	//UpVector = CrossProduct(&ViewDir, &RightVector)*-1;
	up = dir.cross(right)*-1;
}


