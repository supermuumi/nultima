#ifndef __OBJECT3D_HPP__
#define __OBJECT3D_HPP__

#include "vec3.h"

class Object3D {
public:
    Object3D();

    void move(Vec3 dir);
    void moveX(float dist);
    void moveY(float dist);
    void moveZ(float dist);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    //void setView();

    Vec3 pos;
    Vec3 dir;
    Vec3 right;
    Vec3 up;
    float rotX, rotY, rotZ;
};

#endif
