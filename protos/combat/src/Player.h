#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "vec3.h"

class Player {
 public:
    Player(int x, int y);
    
    void render(bool isActive);
    void setPosition(int _x, int _y);

    int m_x, m_y;
    int layer;

private:
    double  m_lastColorChange;
    Vec3    m_color;
};

#endif
