#include "nuGame.h"
#include "nuGlut.h"
#include "nuKeyboard.h"
#include "nuDefs.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuWorld.h"
#include "nuGameState.h"
#include "nuPlayer.h"
#include "nuMapLocation.h"
#include "nuCamera.h"
#include "nuEditor.h"
#include "nuAudioManager.h"
#include "nuLight.h"
#include "nuUtils.h"
#include "nuTimer.h"

#include <cmath>
#include <sstream>

using namespace Nultima;

// TODO [sampo] tsk tsk, should not do allocations in constructor
Game::Game(std::string worldFile, std::string stateFile) :
    m_world(NULL),
    m_state(NULL),
    m_player(NULL),
    m_editor(NULL),
    m_isEditorMode(false),
    m_timeOfDay(0)
{
    m_world = new World(worldFile);
/*
    m_world = new World();
    m_world->generateFromPNG("../../assets/maps/test.png");
    m_world->generateFromPNG("../../assets/maps/south_helsinki.png");
*/
    m_state = new GameState(stateFile);
    m_editor = new Editor(m_world);

    MapLocation playerLocation;
    if (!m_state->getPlayerLocation(playerLocation))
        playerLocation = m_world->getPlayerStart();

    m_player = new Player(playerLocation, m_world);

    // some test audio for verifying bass works
    m_audio = new AudioManager();
    m_audio->loadJSON("../../assets/audio/audio.json");

    m_light = new Light(
        Vec3(0.0f, 0.0f, 5.0f),
        Vec3(.8f, .8f, .8f),
        Vec3(.8f, .8f, .8f),
        Vec3(.8f, .8f, .8f)
        );
}

Game::~Game()
{
    delete m_light;

    delete m_editor;
    delete m_player;
    delete m_state;
    delete m_world;

    delete m_audio;
}

void Game::mainloop()
{
    //m_audio->playMusic("bgmusic");

    Graphics* graphics = Context::get()->getGraphics();
    GLUT::mainloop(this, graphics);
}

void Game::tick()
{
    ScopedTimer timer("Game::tick");

    m_advanceTurn = false;

    // Handle input
    handleKeyboard();
    handleMouse();

    // Streaming

    if (m_advanceTurn)
        processTurn();
}

void Game::display()
{
    ScopedTimer timer("Game::display");

    // prepare
    beginFrame();

    // render
    renderViewport();

    endFrame();
}

void Game::beginFrame()
{
    Graphics* graphics = Context::get()->getGraphics();
    graphics->clear();
}

void Game::endFrame()
{
    Graphics* graphics = Context::get()->getGraphics();

    // Swap
    graphics->swap();
}

void Game::setupLight()
{
    ScopedTimer timer("Game::setupLight");

    // really really really shitty placeholder lighting
    // TODO replace with something proper
    double lightTimer = Utils::getCurrentTime() * 0.001;
    Vec3i pPos = m_player->getPosition();
    m_light->m_pos.m_x = (float)pPos.m_x;
    m_light->m_pos.m_y = (float)pPos.m_y;
    m_light->m_pos.m_z = (float)pPos.m_z + (float)std::sin(lightTimer)*1.0f;
}

void Game::renderViewport()
{
    ScopedTimer timer("Game::renderViewport");

    Graphics* g = Context::get()->getGraphics();
    g->setColor(1.0, 1.0, 1.0, 1.0);

    setupLight();

    // render world
    m_player->render(m_isEditorMode ? m_editor->getCamera() : NULL, m_light);

    if (m_isEditorMode) 
        m_editor->render();

    renderHUD();

    renderStats();
}

void Game::renderHUD()
{
    //printf("m_light = %.2f %.2f %.2f\n", m_light->m_pos.m_x, m_light->m_pos.m_y, m_light->m_pos.m_z);
    // TODO set ortho
    // TODO render time of day
    // TODO render party members & statuses
    // TODO set perspective
}

void Game::renderStats()
{
    std::ostringstream text;
    text << "Game::tick: " <<  m_timers["Game::tick"] << "ms\n";
    text << "Game::display: " <<  m_timers["Game::display"] << "ms\n";
    text << "  Game::renderViewport: " <<  m_timers["Game::renderViewport"] << "ms\n";
    text << "    Game::setupLight: " <<  m_timers["Game::setupLight"] << "ms\n";
    text << "    Player::renderWorld: " <<  m_timers["Player::renderWorld"] << "ms\n";
    text << "      Block::render: " <<  m_timers["Block::render"] << "ms\n";
    text << "    Player::renderPlayer: " <<  m_timers["Player::renderPlayer"] << "ms\n";
    text << "    Game::renderStats: " <<  m_timers["Game::renderStats"] << "ms\n";

    ScopedTimer timer("Game::renderStats");

    Context* context = Context::get();
    Graphics* g = context->getGraphics();

    Vec2i wDim = g->getWindowDimensions();
    g->setOrthoProjection(0, wDim.m_x, 0, wDim.m_y);
    g->setDepthTest(false);
    g->setColor(1.0f, 1.0f, 1.0f, 1.0f);
    g->drawString(text.str().c_str(), 550, 20);

    g->setDepthTest(true);
}

/*
 * Process all buffered input
 */
void Game::handleKeyboard()
{
    Keyboard* keyboard = Context::get()->getKeyboard();

    /*
      if (keyboard->isKeyPressed('q')) m_light->m_pos.m_z -= 0.1;
      if (keyboard->isKeyPressed('z')) m_light->m_pos.m_z += 0.1;
      if (keyboard->isKeyPressed('w')) m_light->m_pos.m_y -= 0.1;
      if (keyboard->isKeyPressed('s')) m_light->m_pos.m_y += 0.1;
      if (keyboard->isKeyPressed('a')) m_light->m_pos.m_x -= 0.1;
      if (keyboard->isKeyPressed('d')) m_light->m_pos.m_x += 0.1;
    */
    while (keyboard->hasKeyPresses())
    {
        int key = keyboard->processKeyPress();

        // TODO this should be wrapped inside e.g. #ifdef NU_ENABLE_EDITOR 
        if (key == NU_KEY_TAB) 
        {
            //m_audio->playEffect("freefall", true);
            m_isEditorMode = !m_isEditorMode;
        }
        else
        {
            if (m_isEditorMode)
                m_editor->handleKeypress(key);
            else
                handleKeypress(key);
        }
    }
}

void Game::handleMouse()
{
    //Mouse* mouse = Context::get()->getMouse();
    if (m_isEditorMode)
        m_editor->handleMouse();

}

/*
 * Handle all keypresses
 */
void Game::handleKeypress(int key)
{
    // move player
    if (key == NU_KEY_LEFT)
        movePlayer(Vec3i(-1, 0, 0));
    if (key == NU_KEY_RIGHT)
        movePlayer(Vec3i(1, 0, 0));
    if (key == NU_KEY_UP)
        movePlayer(Vec3i(0, 1, 0));
    if (key == NU_KEY_DOWN)
        movePlayer(Vec3i(0, -1, 0));
    // TODO handle climbing up/down


    // TODO process other keys like r=rest etc.
}

/*
 * Move player about in the world
 */
void Game::movePlayer(Vec3i d) 
{
    m_player->move(d);
    m_advanceTurn = true;
}

void Game::processTurn()
{
    // TODO move nearby monsters
    // TODO move inhabitants

    // advance time of day
    // TODO Muumi - This is now .16 fixed, I dunno why really.
    m_timeOfDay += NU_GAME_TIMEPERTURN;

    // TODO move sun, e.g. m_world->setTimeOfDay(m_timeOfDay/65536.0f);
}

void Game::processTimers()
{
    Timer* timer = Context::get()->getTimer();
    m_timers["Game::tick"] = timer->getTimerValue("Game::tick");
    m_timers["Game::display"] = timer->getTimerValue("Game::display");
    m_timers["Game::renderViewport"] = timer->getTimerValue("Game::renderViewport");
    m_timers["Game::setupLight"] = timer->getTimerValue("Game::setupLight");
    m_timers["Player::renderWorld"] = timer->getTimerValue("Player::renderWorld");
    m_timers["Block::render"] = timer->getTimerValue("Block::render");
    m_timers["Player::renderPlayer"] = timer->getTimerValue("Player::renderPlayer");
    m_timers["Game::renderStats"] = timer->getTimerValue("Game::renderStats");

    timer->reset("Block::render");
}
