#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "object3d.h"
#include "vec3.h"

class Camera : public Object3D {
public:
    Camera();

    void setView();
};

#endif
