#include "nuPlayer.h"
#include "nuWorld.h"
#include "nuGraphics.h"
#include "nuContext.h"
#include "nuCamera.h"
#include "nuObject.h"
#include "nuBlock.h"

using namespace Nultima;

Player::Player(MapLocation location)
{
    m_location = location;
}

// TODO [sampo] frustum culling
void Player::render(World* world, Camera* inCamera)
{
    Graphics* graphics = Context::get()->getGraphics();
    graphics->setViewProjection();

    // place camera
    Camera playerCamera(m_location);
    Camera* camera = (inCamera) ? inCamera : &playerCamera;
    camera->setView();

    std::vector<Cell*> cells = world->getCells();

    std::vector<Object*> visible;

    // loop cells
    for (std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it)
    // loop layers
    for (int i=0; i<=m_layer; i++)
    {
        Cell* cell = *it;

        // loop blocks
        for (int x=0; x<NU_CELL_WIDTH; x++)
        for (int y=0; y<NU_CELL_HEIGHT; y++)
            visible.push_back((Object*)cell->getBlock(x, y, i));
    }

    // TODO [sampo] loop inhabitants
    //     enqueu inhabitant

    // TODO [sampo] loop monsters
    //   enqueue monster

    // render queue
    for (std::vector<Object*>::iterator it = visible.begin(); it != visible.end(); ++it)
        (*it)->render();

    // TODO [sampo] render hud
}