#pragma once

#include "nuMapLocation.h"

#include <string>

namespace Nultima
{

class GameState
{
public:
    GameState(std::string filename) {};
    ~GameState() {};

    bool getPlayerLocation(MapLocation& location) { (void)location; return true; }
};

};