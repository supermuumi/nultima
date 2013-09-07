#pragma once

#include "nuVec2.h"
#include "nuBlock.h"
#include "nuVec3.h"

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

    typedef enum 
    {
        EDITMODE_NONE = 0,
        EDITMODE_PAINT,
        EDITMODE_ERASE
    } EditMode;

public:
    Editor(World *w);
    ~Editor();

    void    render();
    void    moveSelection(int dx, int dy, int dz);
    void    handleKeypress(int key);
    Camera* getCamera();

    void    saveWorld();

private:
    void    paintCurrentBlock();
    void    eraseCurrentBlock();
    void    changeActiveBlockBy(int);
    void    changeEditMode(EditMode m);
    int     getBlocksetStart(int idx);

    Vec3ui   m_location; // TODO replace with a better data type
    Camera*  m_camera;
    World*   m_world;
    EditMode m_editMode;
    int      m_activeBlock;
    bool     m_helpActive;

    Block*   m_cursor;
};

};
