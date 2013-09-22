#pragma once

#include "nuVec3.h"

namespace Nultima
{

class Camera
{
public:
    Camera();
    Camera(Vec3 location);
    ~Camera() {};

    void moveTo(Vec3 v);
    void setView();

    Vec3 getPosition() { return m_pos; }

    float getCullDistance() { return m_cullDistance; }
    void  setCullDistance(float cullDistance) { m_cullDistance = cullDistance; }

private:
    Vec3 m_pos;
    Vec3 m_dir;
    Vec3 m_up;
    Vec3 m_right;

    float m_cullDistance;
};

};
