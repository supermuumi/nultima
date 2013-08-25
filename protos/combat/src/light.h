#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "object3d.h"
#include "vec3.h"
#include "color.h"

class Light : public Object3D {
public:
    Light();

    Color ambient; //0.8f, 0.8f, 0.8f, 1.0f);
    Color diffuse; //(0.8f, 0.8f, 0.8f, 1.0f);
    Color specular;//(0.8f, 0.8f, 0.8f, 1.0f);

};


#endif
