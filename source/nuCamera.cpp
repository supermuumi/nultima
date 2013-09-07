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
    moveToLocation(location);
}

void Camera::moveTo(Vec3ui v)
{
    m_pos = Vec3((float)v.m_x, (float)v.m_y, (float)v.m_z);
    m_dir = Vec3(0, 0, -1);
    m_up = Vec3(0, 1, 0);
    m_right = Vec3(1, 0, 0);
}

void Camera::moveToLocation(MapLocation location)
{
    Vec2i coord;
    location.getCoordinate(coord);

    m_pos = Vec3((float)coord.m_x, (float)coord.m_y, 30);
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
