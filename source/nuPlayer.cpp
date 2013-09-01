#include "nuPlayer.h"
#include "nuWorld.h"
#include "nuGraphics.h"
#include "nuContext.h"
#include "nuCamera.h"

using namespace Nultima;

Player::Player(MapLocation location)
{
    m_location.setCellIdx   (location.getCellIdx());
    m_location.setBlockX    (location.getBlockX ());
    m_location.setBlockY    (location.getBlockY ());
    m_location.setLayer     (location.getLayer  ());
}

void Player::render(World* world, Camera* inCamera)
{
    Graphics* graphics = Context::get()->getGraphics();
    graphics->setViewProjection();

    // place camera
    Camera playerCamera(m_location);
    Camera* camera = (inCamera) ? inCamera : &playerCamera;
    camera->setView();

    // loop cells
    //   loop blocks
    //     loop layers
    //       render block
    NU_UNREF(world);

    // loop inhabitants
    //     render inhabitant

    // loop monsters
    //   render monster

    // render hud
}