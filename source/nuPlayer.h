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
    Player(MapLocation location);
    ~Player() {};

    void        render(World* world, Camera* camera, Light* light);
    void        move  (Vec3i d);

    Vec3i       getPosition() { return m_location.m_position; }

private:
    MapLocation m_location;
    Character*  m_avatar;
};

};
