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

    m_cullDistance = 40;
}

Camera::Camera(Vec3 location)
{
    moveTo(location);
    m_cullDistance = 40;
}

void Camera::moveTo(Vec3 v)
{
    m_pos = Vec3(v.m_x, v.m_y, v.m_z);
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
