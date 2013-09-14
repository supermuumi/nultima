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
#include <cmath>

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
    m_state = new GameState(stateFile);
    m_editor = new Editor(m_world);

    MapLocation playerLocation;
    if (!m_state->getPlayerLocation(playerLocation))
        playerLocation = m_world->getPlayerStart();

    m_player = new Player(playerLocation);

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
    graphics->enableLighting();
    GLUT::mainloop(this, graphics);
}

void Game::tick()
{
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

void Game::renderViewport()
{
    Graphics* g = Context::get()->getGraphics();
    g->setColor(1.0, 1.0, 1.0, 1.0);

    // really really really shitty placeholder lighting
    // TODO replace with something proper
    double timer = Utils::getCurrentTime() * 0.001;
    Vec3i pPos = m_player->getPosition();
    m_light->m_pos.m_x = pPos.m_x;
    m_light->m_pos.m_y = pPos.m_y;
    m_light->m_pos.m_z = pPos.m_z + std::sin(timer)*5.0f;

    // render world
    m_player->render(m_world, m_isEditorMode ? m_editor->getCamera() : NULL, m_light);

    if (m_isEditorMode) 
        m_editor->render();

    renderHUD();
}

void Game::renderHUD()
{
    //printf("m_light = %.2f %.2f %.2f\n", m_light->m_pos.m_x, m_light->m_pos.m_y, m_light->m_pos.m_z);
    // TODO set ortho
    // TODO render time of day
    // TODO render party members & statuses
    // TODO set perspective
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
