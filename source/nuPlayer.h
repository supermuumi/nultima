#pragma once

#include "nuMapLocation.h"

namespace Nultima
{

class World;
class Camera;

class Player
{
public:
    Player(MapLocation location);
    ~Player() {};

    void        render(World* world, Camera* camera);
    void        move  (int dx, int dy);

private:
    MapLocation m_location;
};

};
