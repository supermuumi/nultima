#pragma once

#include <string>

namespace Nultima
{

class World;
class GameState;
class Player;
class Camera;

class Game
{
public:
    Game(std::string worldFile, std::string stateFile);
    ~Game();

    void mainloop();
    void display();
    void tick();

private:
    void handleKeyboard();
    void beginFrame();
    void endFrame();

    World*      m_world;
    GameState*  m_state;
    Player*     m_player;
    Camera*     m_editorCamera;
    bool        m_isEditorMode;
};

}; // namespace
