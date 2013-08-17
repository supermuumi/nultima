#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "object3d.h"
#include "vec3.h"


// frustum culling return values
#define FRUSTUM_OUT 0
#define FRUSTUM_IN 1
#define FRUSTUM_INTERSECT 2

class Camera : public Object3D {
 public:
    Camera();

    void setView();

    void extractFrustum();
    bool pointInFrustum(float x, float y, float z);
    bool sphereInFrustum(float x, float y, float z, float radius);
    bool cubeInFrustum(float x, float y, float z, float size);

 private:
    float frustum[6][4];
};

#endif
