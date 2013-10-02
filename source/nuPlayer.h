#pragma once

#include "nuVec3.h"

namespace Nultima
{

class World;
class Camera;
class Character;
class Light;

class Player
{
public:
    Player(Vec3i position, World* world);
    ~Player() {};

    void        render  ();
    bool        move    (Vec3i d);
    void        tick    ();

    void        setPosition(Vec3i v) { m_position = v; }
    Vec3i       getPosition()        { return m_position; }
    Camera*     getCamera()          { return m_camera; }

private:
    Vec3i       m_position;
    Character*  m_avatar;
    World*      m_world;
    Camera*     m_camera;
};

};
