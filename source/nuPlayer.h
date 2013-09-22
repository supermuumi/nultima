#pragma once

#include "nuMapLocation.h"

namespace Nultima
{

class World;
class Camera;
class Character;
class Light;

class Player
{
public:
    Player(MapLocation location, World* world);
    ~Player() {};

    void        render  ();
    bool        move    (Vec3i d);
    void        tick    ();

    Vec3i       getPosition() { return m_location.m_position; }
    Camera*     getCamera()     { return m_camera; }

private:
    MapLocation m_location;
    Character*  m_avatar;
    World*      m_world;
    Camera*     m_camera;
};

};
