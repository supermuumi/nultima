#pragma once

namespace Nultima
{

    class Camera;
    class MapLocation;
    class World;


/*
 * requirements:
 * 
 * - move cursor around in the world
 * - move up&down layers
 * - add cells?
 * - change block at (x,y,layer)
 * - view list of blocks
 * - load & save world
 * 
 */
class Editor
{
public:
    Editor(World *w);
    ~Editor();

    void    render();
    void    move(int dx, int dy);
    void    handleKeypress(int key);
    Camera* getCamera();

private:
    int     getBlocksetStart(int idx);

    MapLocation m_location;
    Camera*     m_camera;
    World*      m_world;
    bool        m_paintMode;
    int         m_activeBlock;
};

};
