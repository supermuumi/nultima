#include "nuCamera.h"
#include "nuGraphics.h"
#include "nuContext.h"

using namespace Nultima;

Camera::Camera()
{
    m_pos = Vec3(0, 0, 0);
    m_dir = Vec3(0, 0, -1);
    m_up = Vec3(0, 1, 0);
    m_right = Vec3(1, 0, 0);
}

Camera::Camera(MapLocation location)
{
    int cellX = location.getCellX();
    int cellY = location.getCellY();
    int blockX = location.getBlockX();
    int blockY = location.getBlockY();

    int posX = cellX * NU_CELL_WIDTH + blockX;
    int posY = cellY * NU_CELL_HEIGHT + blockY;

    m_pos = Vec3((float)posX, (float)posY, 0);
    m_dir = Vec3(0, 0, -1);
    m_up = Vec3(0, 1, 0);
    m_right = Vec3(1, 0, 0);
}

void Camera::setView()
{
    Vec3 viewPoint = m_pos + m_dir;

    Graphics* graphics = Context::get()->getGraphics();
    graphics->lookAt(m_pos, viewPoint, m_up);
}
