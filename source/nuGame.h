#pragma once

#include <string>

#define NU_GAME_TIMEPERTURN 2048

namespace Nultima
{

class World;
class GameState;
class Player;
class Camera;
class Editor;

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
    void movePlayer(int dx, int dy, int dz);
    void handleKeyboard();
    void handleKeypress(int key);
    void processTurn();

    World*      m_world;
    GameState*  m_state;
    Player*     m_player;
    Editor*     m_editor;
    bool        m_isEditorMode;
    bool        m_advanceTurn;
    int         m_timeOfDay; // .16 fixed? :-P

};

}; // namespace
