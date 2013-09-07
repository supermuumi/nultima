#include "nuCamera.h"
#include "nuEditor.h"
#include "nuKeyboard.h"
#include "nuWorld.h"

using namespace Nultima;

Editor::Editor(World *world)
{
    m_world = world;
    m_camera = new Camera(m_location);
    m_paintMode = false;
    m_activeBlock = 0;
}

Editor::~Editor()
{
    delete m_camera;
}

Camera* Editor::getCamera()
{
    return m_camera;
}

void Editor::render()
{
    // TODO draw cursor over active block
    // TODO render hud
}

void Editor::move(int dx, int dy)
{
    // TODO move active cell around
    m_location.move(dx, dy);
    m_camera->moveToLocation(m_location);
}

void Editor::handleKeypress(int key)
{
    if (key == NU_KEY_LEFT) move(-1, 0);
    if (key == NU_KEY_RIGHT) move(1, 0); 
    if (key == NU_KEY_UP) move(0, 1); 
    if (key == NU_KEY_DOWN) move(0, -1); 
    
    // painting blocks
    if (key == 'p') m_paintMode = !m_paintMode;
    
    // selecting blocks
    if (key == 'q') m_activeBlock--;
    if (key == 'w') m_activeBlock++;
    if (key >= '1' && key <= '9') m_activeBlock = getBlocksetStart(key-'1');
}

/*
 * Return index of first block in a predefined "set" of blocks.
 * For example, 0=water tiles, 1=grassland etc, 2=roads and so forth.
 */
int Editor::getBlocksetStart(int idx) 
{
    // TODO
    return 0;
}
