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
    // rendering
    void beginFrame();
    void endFrame();
    void renderViewport();
    void renderHUD();

    // actual gameplay related
    void handleKeyboard();
    void processTurn();


    World*      m_world;
    GameState*  m_state;
    Player*     m_player;
    Camera*     m_editorCamera;
    bool        m_isEditorMode;
};

}; // namespace
