#pragma once

#include "nuVec3.h"

#include <string>
#if defined(__APPLE__) || defined(MACOSX)
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

#define NU_GAME_TIMEPERTURN 2048

namespace Nultima
{

class World;
class GameState;
class Player;
class Camera;
class Editor;
class AudioManager;
class Light;

class Game
{
public:
    Game(std::string worldFile, std::string stateFile);
    ~Game();

    void mainloop();
    void display();
    void tick();
    void processTimers();

private:
    // rendering
    void beginFrame();
    void endFrame();
    void renderViewport();
    void renderHUD();
    void renderStats();
    void setupLight();

    // actual gameplay related
    void movePlayer(Vec3i d);
    void handleKeyboard();
    void handleMouse();
    void handleKeypress(int key);
    void processTurn();

    World*          m_world;
    GameState*      m_state;
    Player*         m_player;
    Editor*         m_editor;
    bool            m_isEditorMode;
    bool            m_advanceTurn;
    int             m_timeOfDay; // .16 fixed? :-P
    AudioManager*   m_audio;
    Light*          m_light;
    std::tr1::unordered_map<std::string, double> m_timers;
};

}; // namespace
