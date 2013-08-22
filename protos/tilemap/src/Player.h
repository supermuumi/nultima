#ifndef __PLAYER_H__
#define __PLAYER_H__

class Player {
 public:
    Player();
    
    void render();
    void setPosition(int _x, int _y);

    int x, y;
    int layer;
};

#endif
