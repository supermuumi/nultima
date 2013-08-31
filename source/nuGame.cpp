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

using namespace Nultima;

// TODO [sampo] tsk tsk, should not do allocations in constructor
Game::Game(std::string worldFile, std::string stateFile)
{
    m_world = new World(worldFile);
    m_state = new GameState(stateFile);

    MapLocation playerLocation;
    if (!m_state->getPlayerLocation(playerLocation))
        m_world->getPlayerStart(playerLocation);

    m_player = new Player(playerLocation);
}

Game::~Game()
{
    delete m_player;
    delete m_state;
    delete m_world;
}

void Game::mainloop()
{
    Graphics* graphics = Context::get()->getGraphics();
    GLUT::mainloop(this, graphics);
}

void Game::display()
{
    Graphics* graphics = Context::get()->getGraphics();

    // Handle input
    // TODO [sampo] This is not really related to rendering, do earlier?
    handleKeyboard();

    // Clear
    graphics->clear();

    // render
    m_player->render();

    // Swap
    graphics->swap();
}

void Game::handleKeyboard()
{
    Keyboard* keyboard = Context::get()->getKeyboard();
    NU_UNREF(keyboard);
}
