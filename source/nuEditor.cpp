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

using namespace Nultima;

Editor::Editor(World *world) :
    m_cameraOffset(0, 0, 5)
{
    m_world = world;
    m_location = Vec3i(0, 0, 0);
    m_camera = new Camera();
    m_camera->moveTo(m_cameraOffset);
    m_editMode = EDITMODE_NONE;
    m_activeBlock = Block::ROCK;
    m_helpActive = false;
    m_cursor = new Block(m_activeBlock, Vec3i(0, 0, 0));
}

Editor::~Editor()
{
    delete m_camera;
    delete m_cursor;
}

Camera* Editor::getCamera()
{
    return m_camera;
}

void Editor::render()
{
    Graphics* g = Context::get()->getGraphics();

    // draw active block over map (alternating every 300ms)
    int timer = (int)Utils::getCurrentTime();
//    if (timer % 600 < 300)
//    {
        g->pushMatrix();
        // offset cursor block slightly so it actually shows
        g->translate(0, 0, 0.1f);
        m_cursor->render();
        g->popMatrix();
//    }

    // TODO render hud
    if (m_helpActive)
    {
        // TODO set ortho view
        // TODO display commands
        // TODO set normal view
    }

    // render stats
    char str[128];
    sprintf(str, "Block=[%d,%d] tile=%d\nlayer=%d", m_location.m_x, m_location.m_y, m_activeBlock, m_location.m_z);
    g->setColor(1.0, 1.0, 1.0, 1.0);
    g->drawString(str, 20, 20);
}

void Editor::moveSelection(Vec3i d)
{
    m_location = m_location + d;
    // TODO Vec3ui means we never go <0, but rather get -1>NU_MAX_LAYERS which makes layer switching wrap around
    if (m_location.m_z < 0)
        m_location.m_z = 0;
    if (m_location.m_z >= NU_MAX_LAYERS)
        m_location.m_z = NU_MAX_LAYERS-1;

    m_cursor->moveTo(Vec3i(m_location.m_x, m_location.m_y, m_location.m_z));
    m_camera->moveTo(m_location + m_cameraOffset);

    if (m_editMode == EDITMODE_PAINT)
        paintCurrentBlock();

    if (m_editMode == EDITMODE_ERASE)
        eraseCurrentBlock();
}

void Editor::changeActiveBlockBy(int delta) {
    m_activeBlock += delta;
    // TODO proper values
    if (m_activeBlock < 0) 
        m_activeBlock = 0;

    if (m_activeBlock > 37) //Block::ROCK)
        m_activeBlock = 37; //Block::ROCK;

    m_cursor->setType(m_activeBlock);
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

    // misc
    if (key == 'h') m_helpActive = !m_helpActive;
    
    // painting blocks
    if (key == 'p') changeEditMode(EDITMODE_PAINT);
    if (key == 'e') changeEditMode(EDITMODE_ERASE);
    if (key == 's') paintCurrentBlock();
    if (key == 'd') eraseCurrentBlock();
    
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
    Block* block = new Block(m_activeBlock, Vec3i(m_location.m_x, m_location.m_y, m_location.m_z));
    m_world->insertBlock(block);
}

void Editor::eraseCurrentBlock()
{
    m_world->clearBlock(m_location);
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
