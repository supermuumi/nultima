#include "nuGame.h"
#include "nuGlut.h"
#include "nuKeyboard.h"
#include "nuDefs.h"
#include "nuContext.h"
#include "nuGraphics.h"

using namespace Nultima;

Game::Game(std::string worldFile, std::string stateFile)
{
}

Game::~Game()
{
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

    // Swap
    graphics->swap();
}

void Game::handleKeyboard()
{
    Keyboard* keyboard = Context::get()->getKeyboard();
    NU_UNREF(keyboard);
}
