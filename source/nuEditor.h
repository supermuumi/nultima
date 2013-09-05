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

    void    render   ();
    void    move     (int dx, int dy);
    Camera* getCamera() { return m_camera; }

private:
    MapLocation m_location;
    Camera*     m_camera;
    World*      m_world;
};

};
