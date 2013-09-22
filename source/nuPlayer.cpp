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

Player::Player(Vec3i position, World* world)
{
    m_world = world;
    m_position = position;
    m_avatar = new Character();
    Vec3 cameraPosition((float)m_position.m_x, (float)m_position.m_y, (float)m_position.m_z);
    cameraPosition.m_z += 10; 
    m_camera = new Camera(cameraPosition);
}

void Player::render()
{
    ScopedTimer timer("Player::renderPlayer");

    Graphics* g = Context::get()->getGraphics();
    // Render player
    g->pushMatrix();
    g->translate((float)m_position.m_x+0.5f, (float)m_position.m_y+0.5f, (float)m_position.m_z+0.5f);
    m_avatar->render();
    g->popMatrix();
}

void Player::tick()
{
    m_avatar->tick();
}

bool Player::move(Vec3i d)
{
    Tilemap* tilemap = Context::get()->getTexManager()->getTilemap();

    // Current block
    Block* currentBlock = m_world->getBlockAt(m_position);

    // Target block
    Vec3i targetPosition = m_position + d;
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

    m_position = targetPosition;

    Vec3 cameraPos((float)m_position.m_x, (float)m_position.m_y, (float)m_position.m_z+10);
    m_camera->moveTo(cameraPos);
    return true;
}
