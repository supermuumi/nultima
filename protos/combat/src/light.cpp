#include <math.h>

#include "object3d.h"
#include "light.h"
#include "vec3.h"

Light::Light() : ambient(.8f, .8f, .8f, 1.f), diffuse(.8f, .8f, .8f, 1.f), specular(.8f, .8f, .8f, 1.f) {
    pos = Vec3(.0f, .0f, .0f);
    dir = Vec3(.0f, .0f, -1.f);
    right = Vec3(1.f, .0f, .0f);
    up = Vec3(.0f, 1.f, 0.f);

    rotX = rotY = rotZ = .0f;
}
