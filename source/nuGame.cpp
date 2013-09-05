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
    m_world = new World("foo.world");
    m_state = new GameState(stateFile);
    m_editor = new Editor(m_world);

    MapLocation playerLocation;
    if (!m_state->getPlayerLocation(playerLocation))
        playerLocation = m_world->getPlayerStart();

    m_player = new Player(playerLocation);
}

Game::~Game()
{
    delete m_editor;
    delete m_player;
    delete m_state;
    delete m_world;
}

void Game::mainloop()
{
    Graphics* graphics = Context::get()->getGraphics();
    GLUT::mainloop(this, graphics);
}

void Game::tick()
{
    // Handle input
    handleKeyboard();

    // Streaming
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
    // render world
    m_player->render(m_world, m_isEditorMode ? m_editor->getCamera() : NULL);

    renderHUD();
}

void Game::renderHUD()
{
    // TODO set ortho
    // TODO render time of day
    // TODO render party members & statuses
}

void Game::handleKeyboard()
{
    Keyboard* keyboard = Context::get()->getKeyboard();
    NU_UNREF(keyboard);

    int dx = 0;
    int dy = 0;
    
    while (keyboard->hasKeyPresses())
    {
        int key = keyboard->processKeyPress();

        switch(key)
        {
            // player movement
            case NU_KEY_LEFT:  dx--; break;
            case NU_KEY_RIGHT: dx++; break;
            case NU_KEY_UP:    dy++; break; 
            case NU_KEY_DOWN:  dy--; break;

            case NU_KEY_TAB: m_isEditorMode != m_isEditorMode; break;
            // TODO process other keys like r=rest etc.

        }
    }

#ifdef NU_ENABLE_KEYHOLD
    if (keyboard->isKeyPressed(NU_KEY_LEFT)) dx--;
    if (keyboard->isKeyPressed(NU_KEY_RIGHT)) dx++;
    if (keyboard->isKeyPressed(NU_KEY_UP)) dy++;
    if (keyboard->isKeyPressed(NU_KEY_DOWN)) dy--;
#endif
    if (dx != 0 || dy != 0)
    {
        m_player->move(dx, dy);
        processTurn();
    }
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
