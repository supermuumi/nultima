#pragma once

#include "nuVec3.h"

#include <string>
#include <vector>

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
class Monster;
class NPC;

class Game
{
public:
    Game(std::string worldFile, std::string stateFile);
    ~Game();

    void mainloop();
    void display();
    void tick();
    void processTimers();

    Player* getPlayer() { return m_player; }

private:
    // rendering
    void beginFrame     ();
    void endFrame       ();
    void renderViewport ();
    void renderWorld    (Camera* camera);
    void renderHUD      ();
    void renderStats    ();
    void setupLight     ();

    // actual gameplay related
    void movePlayer(Vec3i d);
    void handleKeyboard();
    void handleMouse();
    void handleKeypress(int key);
    void processTurn();

    // party related
    void addNPCToParty(NPC*);
    void removeNPCFromParty(std::string name);

    // pathfinding etc
    bool findPath(Vec3i start, Vec3i goal);

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

    std::vector<Monster*> m_monsters;
    std::vector<NPC*>     m_npcs;
    std::vector<NPC*>     m_party;
};

}; // namespace
