#pragma once

#include <string>

namespace Nultima
{

class GameState
{
public:
    GameState(std::string filename) {};
    ~GameState() {};

    bool getPlayerLocation(Vec3i& location) { (void)location; return false; }
};

};