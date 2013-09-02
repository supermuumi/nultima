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

using namespace Nultima;

// TODO [sampo] tsk tsk, should not do allocations in constructor
Game::Game(std::string worldFile, std::string stateFile) :
    m_world(NULL),
    m_state(NULL),
    m_player(NULL),
    m_editorCamera(NULL),
    m_isEditorMode(false)
{
    m_world = new World("foo.world");
    m_state = new GameState(stateFile);

    MapLocation playerLocation;
    if (!m_state->getPlayerLocation(playerLocation))
        playerLocation = m_world->getPlayerStart();

    m_player = new Player(playerLocation);

    m_editorCamera = new Camera();
}

Game::~Game()
{
    delete m_editorCamera;
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
    m_player->render(m_world, m_isEditorMode ? m_editorCamera : NULL);

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

void Game::handleKeyboard()
{
    Keyboard* keyboard = Context::get()->getKeyboard();
    NU_UNREF(keyboard);
}
