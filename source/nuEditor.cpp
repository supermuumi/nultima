#include "nuCamera.h"
#include "nuEditor.h"
#include "nuWorld.h"

using namespace Nultima;

Editor::Editor(World *world)
{
    m_world = world;
    m_camera = new Camera();
}

Editor::~Editor()
{
    delete m_camera;
}

void Editor::render()
{
// TODO render hud
}

void Editor::move(int dx, int dy)
{
// TODO move active cell around
}
