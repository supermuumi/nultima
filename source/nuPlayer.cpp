#include "nuPlayer.h"
#include "nuWorld.h"
#include "nuGraphics.h"
#include "nuContext.h"
#include "nuCamera.h"
#include "nuObject.h"
#include "nuBlock.h"
#include "nuCell.h"

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

    // loop cells
    for (std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it)
    // loop layers
    for (int i=0; i<NU_MAX_LAYERS; i++)
    {
        Cell* cell = (*it);
        cell->beginRendering();
        // loop blocks
        for (int x=0; x<NU_CELL_WIDTH; x++)
        for (int y=0; y<NU_CELL_HEIGHT; y++)
        {
            const Block* block = cell->getBlock(x, y, i);
            if (block)
                block->render();
        }
        cell->endRendering();
    }

    // TODO [sampo] Render player

    // TODO [sampo] loop inhabitants
    //     render inhabitant

    // TODO [sampo] loop props
    //     render props

    // TODO [sampo] loop monsters
    //   render monster
}

void Player::move(int dx, int dy, int dz)
{
    m_location.move(dx, dy, dz);
}
