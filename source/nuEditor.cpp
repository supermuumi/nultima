#include "nuCamera.h"
#include "nuEditor.h"
#include "nuKeyboard.h"
#include "nuWorld.h"
#include "nuContext.h"
#include "nuVec2.h"
#include "nuCell.h"
#include "nuModel.h"
#include "nuGraphics.h"
#include "nuUtils.h"
#include "nuMinimap.h"
#include "nuTexManager.h"

using namespace Nultima;

Editor::Editor(World *world) :
    m_cameraOffset(0, 0, 5)
{
    m_world = world;
    m_camera = new Camera();
    m_camera->moveTo(m_cameraOffset);
    m_editMode = EDITMODE_NONE;
    m_helpActive = false;
    m_cursor = Vec3i(0, 0, 0);
    m_cursorType = Block::ROCK;
    m_minimap = new Minimap(m_world);
    m_minimap->update();
}

Editor::~Editor()
{
    delete m_camera;
}

Camera* Editor::getCamera()
{
    return m_camera;
}

void Editor::renderHud()
{
    Graphics* g = Context::get()->getGraphics();
    Vec2i wDim = g->getWindowDimensions();

    // set ortho view
    g->setOrthoProjection(0, wDim.m_x, 0, wDim.m_y);

    // Render minimap
    m_minimap->render();

	if (m_helpActive)
    {
        // TODO display commands
        // TODO set normal view
    }

    // render stats
    char str[128];
    sprintf(str, "Block=[%d,%d]\nlayer=%d", m_cursor.m_x, m_cursor.m_y, m_cursor.m_z);
    g->setColor(1.0, 1.0, 1.0, 1.0);
    g->drawString(str, 20, 20);
}

void Editor::renderActiveBlock()
{
    Graphics* g = Context::get()->getGraphics();

    int timer = (int)Utils::getCurrentTime();
    if (timer % 600 < 300)
    {
        g->pushMatrix();
        // offset cursor block slightly so it actually shows
        g->translate(0, 0, 0.1f);
        Block b = Block(m_cursorType, m_cursor);
        b.render();
        g->popMatrix();
    }
}

void Editor::render()
{
    // draw active block over map (alternating every 300ms)
    renderActiveBlock();

    // render hud
    renderHud();
}

void Editor::moveSelection(Vec3i d)
{
    m_cursor = m_cursor + d;
    // TODO Vec3ui means we never go <0, but rather get -1>NU_MAX_LAYERS which makes layer switching wrap around
    if (m_cursor.m_z < 0)
        m_cursor.m_z = 0;
    if (m_cursor.m_z >= NU_MAX_LAYERS)
        m_cursor.m_z = NU_MAX_LAYERS-1;

    m_camera->moveTo(m_cursor + m_cameraOffset);

    if (m_editMode == EDITMODE_PAINT)
        paintCurrentBlock();

    if (m_editMode == EDITMODE_ERASE)
        eraseCurrentBlock();
}

void Editor::moveCamera(Vec3i d)
{
    m_cameraOffset = m_cameraOffset + d;
    m_camera->moveTo(m_cursor + m_cameraOffset);
}

void Editor::changeActiveBlockBy(int delta) {
    m_cursorType += delta;

    TexManager* tex = Context::get()->getTexManager();
    if (m_cursorType < 0) 
        m_cursorType = tex->getNumTiles()-1;
    if (m_cursorType >= tex->getNumTiles())
        m_cursorType = 0;
}

void Editor::handleKeypress(int key)
{
    // move cursor
    if (key == NU_KEY_LEFT) moveSelection(Vec3i(-1, 0, 0));
    if (key == NU_KEY_RIGHT) moveSelection(Vec3i(1, 0, 0)); 
    if (key == NU_KEY_UP) moveSelection(Vec3i(0, 1, 0)); 
    if (key == NU_KEY_DOWN) moveSelection(Vec3i(0, -1, 0));
    if (key == '.') moveSelection(Vec3i(0, 0, 1));
    if (key == ',') moveSelection(Vec3i(0, 0, -1));

    // move camera
    if (key == NU_KEY_PAGE_UP) moveCamera(Vec3i(0, 0, 1));
    if (key == NU_KEY_PAGE_DOWN) moveCamera(Vec3i(0, 0, -1));


    // misc
    if (key == 'h') m_helpActive = !m_helpActive;
    
    // painting blocks
    if (key == 'p') changeEditMode(EDITMODE_PAINT);
    if (key == 'e') changeEditMode(EDITMODE_ERASE);
    if (key == 's') paintCurrentBlock();
    if (key == 'd') eraseCurrentBlock();

    // minimap
    if (key == 'm') m_minimap->update();
    
    // change block type
    if (key == 'q') changeActiveBlockBy(-1);
    if (key == 'w') changeActiveBlockBy(1);
    //if (key >= '1' && key <= '9') m_activeBlock = getBlocksetStart(key-'1');

    // saving & loading
    // TODO change to something that doesn't overlap with "blockset" selection
    if (key == '5') saveWorld();

}

void Editor::changeEditMode(EditMode newMode)
{
    if (newMode == EDITMODE_PAINT)
    {
        m_editMode = (m_editMode == EDITMODE_PAINT) ? EDITMODE_NONE : EDITMODE_PAINT;
        if (m_editMode == EDITMODE_PAINT)
            paintCurrentBlock();
    }

    if (newMode == EDITMODE_ERASE)
    {
        m_editMode = (m_editMode == EDITMODE_ERASE) ? EDITMODE_NONE : EDITMODE_ERASE;
        if (m_editMode == EDITMODE_ERASE)
            eraseCurrentBlock();
    }
}

void Editor::paintCurrentBlock()
{
    Block* block = new Block(m_cursorType, m_cursor);
    m_world->insertBlock(block);
}

void Editor::eraseCurrentBlock()
{
    m_world->clearBlock(m_cursor);
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

void Editor::saveWorld()
{
    Context* context = Context::get();
    std::string fileName = context->getWorldFile();
    // TODO [sampo] use fileUtils
    std::ofstream worldFile(fileName.c_str(), std::ios_base::binary);
    m_world->serialize(&worldFile);
    worldFile.close();
}
