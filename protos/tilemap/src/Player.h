#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Inventory.h"

#define PLAYER_ATTR_DEFAULT 12
#define PLAYER_ATTR_MIN 1
#define PLAYER_ATTR_MAX 20

class Player {
 public:
    Player();
    ~Player();
    
    void render();
    void setPosition(int _x, int _y);

    // attributes
    int m_strength;
    int m_dexterity;
    int m_intelligence;

    Inventory *inventory;

    int x, y;
    int layer;
};

#endif
