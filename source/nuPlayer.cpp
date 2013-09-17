#include "nuPlayer.h"
#include "nuWorld.h"
#include "nuGraphics.h"
#include "nuContext.h"
#include "nuCamera.h"
#include "nuObject.h"
#include "nuBlock.h"
#include "nuCell.h"
#include "nuCharacter.h"
#include "nuLight.h"
#include "nuDefs.h"
#include "nuTexManager.h"
#include "nuTilemap.h"
#include "nuTimer.h"

using namespace Nultima;

Player::Player(MapLocation location, World* world)
{
    m_world = world;
    m_location = location;
    m_avatar = new Character();
}

void Player::setupWorldRendering(Camera* inCamera)
{
    // TODO [sampo] tick outside?
    m_avatar->tick();

    Graphics* g = Context::get()->getGraphics();
    g->setPerspectiveProjection();

    // place camera
    Camera playerCamera(m_location);
    Camera* camera = (inCamera) ? inCamera : &playerCamera;
    camera->setView();

    g->enableLighting();
    g->setLight(m_light);
}

void Player::renderWorld()
{
    ScopedTimer timer("Player::renderWorld");

    std::tr1::unordered_map<unsigned int, Cell*> cells = m_world->getCells();

    Vec3i pos = m_location.m_position;

    // loop cells
    for (std::tr1::unordered_map<unsigned int, Cell*>::iterator it = cells.begin(); it != cells.end(); ++it)
    // loop layers
    for (int i=0; i<NU_MAX_LAYERS; i++)
    {
        Cell* cell = it->second;
        // loop blocks

        if (cell)
        {
            for (int x=0; x<NU_CELL_WIDTH; x++)
            for (int y=0; y<NU_CELL_HEIGHT; y++)
            {
                // TODO [muumi] do not render blocks that are above player
                /*
                if (i > pos.m_z && x == pos.m_x && y == pos.m_y)
                    continue;
                */
                const Block* block = cell->getBlock(Vec3i(x, y, i));
                if (block)
                    block->render();
            }
        }
    }
}

void Player::renderPlayer()
{
    ScopedTimer timer("Player::renderPlayer");

    Graphics* g = Context::get()->getGraphics();
    // Render player
    g->pushMatrix();
    g->translate((float)m_location.m_position.m_x+0.5f, (float)m_location.m_position.m_y+0.5f, (float)m_location.m_position.m_z+0.5f);
    m_avatar->render();
    g->popMatrix();
}

void Player::finishWorldRendering()
{
    Graphics* g = Context::get()->getGraphics();
    g->disableLighting();
}

// TODO [sampo] frustum culling
void Player::render(Camera* inCamera, Light* light)
{
    m_light = light;

    setupWorldRendering(inCamera);
    renderWorld();
    renderPlayer();
    finishWorldRendering();
}

bool Player::move(Vec3i d)
{
    Tilemap* tilemap = Context::get()->getTexManager()->getTilemap();

    // Current block
    Block* currentBlock = m_world->getBlockAt(m_location.m_position);

    // Target block
    Vec3i targetPosition = m_location.m_position + d;
    // Player must always be on at least layer 1
    NU_ASSERT(targetPosition.m_z > 0);
    Block* targetBlock = m_world->getBlockAt(targetPosition);

    // Below block
    Vec3i below = targetPosition;
    below.m_z -= 1;
    Block* belowBlock = m_world->getBlockAt(below);

    // Drop down a layer
    if (!targetBlock    &&  // Target is an empty block
        !currentBlock   &&  // Current must be empty
        belowBlock      &&  // Below must not be empty
        belowBlock->getRepresentation() == Block::HALFBLOCK &&  // The block below target is a half block
        tilemap->isTileSolid(belowBlock->getType()))            // Below block must be solid
    {
        targetPosition.m_z -= 1;
        targetBlock = m_world->getBlockAt(targetPosition);
    }
    // Rise up a layer
    else if (targetBlock    &&  // Target must not be empty   
             currentBlock   &&  // Current mut not be empty
             currentBlock->getRepresentation()  == Block::HALFBLOCK &&  // Current block must be half block
             targetBlock->getRepresentation()   == Block::BLOCK     &&  // Target block must be a full block
             tilemap->isTileSolid(targetBlock->getType()))              // Target block must be solid
    {
        targetPosition.m_z += 1;
        targetBlock = m_world->getBlockAt(targetPosition);
    }
    // Move onto a half block, does not need a below block
    else if (targetBlock && // Must have a target block
             targetBlock->getRepresentation() == Block::HALFBLOCK)
    {
        // nop
    }
    // Move onto a plane block
    else if (targetBlock    && // Must have a target block
             belowBlock     && // Must have a below block
             targetBlock->getRepresentation() == Block::PLANE   &&  // Target block is plane
             belowBlock->getRepresentation() == Block::BLOCK    &&  // Below block must be block 
             tilemap->isTileSolid(belowBlock->getType()))           // Below block must be solid
    {
        // nop
    }
    // Empty target
    else if (belowBlock     &&
             !targetBlock   &&
             belowBlock->getRepresentation() != Block::HALFBLOCK &&
             tilemap->isTileSolid(belowBlock->getType()))
    {
        // nop
    }
    // Illegal move
    else
    {
        return false;
    }

    m_location.set(targetPosition);
    return true;
}
