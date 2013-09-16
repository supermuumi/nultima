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

    void        render(Camera* camera, Light* light);
    bool        move  (Vec3i d);

    Vec3i       getPosition() { return m_location.m_position; }

private:
    void        renderWorld();
    void        setupWorldRendering(Camera* inCamera);

    MapLocation m_location;
    Character*  m_avatar;
    World*      m_world;
    // TODO [sampo] get this from the world?
    Light*      m_light;
};

};
