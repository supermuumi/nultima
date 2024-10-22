#include "nuContext.h"
#include "nuGame.h"

using namespace Nultima;

int main (int argc, char** argv)
{
    // Context
    Context* context = Context::get();
    context->init(argc, argv);

    // Create World
    Game* game = new Game(context->getWorldFile(), context->getStateFile());
    context->setGame(game);

    // Start update loop
    game->mainloop();

    // Cleanup
    context->deinit();

    delete game;
    context->release();
}
