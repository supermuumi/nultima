#pragma once

#include "nuVec2.h"
#include "nuVec3.h"

namespace Nultima
{

    class Camera;
    class MapLocation;
    class World;
    class Minimap;
    class Block;

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
    void    handleKeypress(int key);
    Camera* getCamera();

private:
    void    paintCurrentBlock();
    void    eraseCurrentBlock();
    void    changeActiveBlockBy(int);
    void    changeEditMode(EditMode m);
    int     getBlocksetStart(int idx);
    void    moveCamera(Vec3i);
    void    moveSelection(Vec3i d);
    void    saveWorld();
    void    renderHud();
    void    renderActiveBlock();

    Camera*     m_camera;
    Vec3i       m_cameraOffset;
    World*      m_world;
    EditMode    m_editMode;
    bool        m_helpActive;

    Vec3i       m_cursor;
    int         m_cursorType;
    Minimap*    m_minimap;
};

};
