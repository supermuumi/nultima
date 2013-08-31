#pragma once

#include <string>

namespace Nultima
{

class World;
class GameState;
class Player;

class Game
{
public:
    Game(std::string worldFile, std::string stateFile);
    ~Game();

    void mainloop();
    void display();

private:
    void handleKeyboard();

    World*      m_world;
    GameState*  m_state;
    Player*     m_player;
};

}; // namespace
