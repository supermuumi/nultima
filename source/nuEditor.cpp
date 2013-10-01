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
#include "nuMouse.h"

#include <string>

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
    m_cursorType = 0;
    m_cursorRepresentation = Block::PLANE;
    m_minimap = new Minimap(m_world);
    m_minimap->update();
}

Editor::~Editor()
{
    delete m_camera;
    delete m_minimap;
}

Camera* Editor::getCamera()
{
    return m_camera;
}

std::string Editor::getEditModeName()
{
    std::string modeNames[] = {
        "None",
        "Paint",
        "Erase",
        "Road"
    };

    return modeNames[m_editMode];
}

void Editor::renderHud()
{
    Graphics* g = Context::get()->getGraphics();
    Vec2i wDim = g->getWindowDimensions();

    // set ortho view
    g->setOrthoProjection(0, wDim.m_x, 0, wDim.m_y);

    if (m_helpActive)
    {
        g->setDepthTest(false);
        g->setColor(0.8f, 0.0f, 0.0f, 0.5f);
        g->fillRect(20.f, 20.f, wDim.m_x-20.f, wDim.m_y-20.f, true);

        std::string helpText = 
            "arrows       - move\n"
            ".            - up layer\n"
            ",            - down layer\n"
            "page up/down - zoom\n"
            "home/end     - cull distance\n"
            "\n"
            "h            - toggle help\n"
            "\n"
            "p            - toggle paint mode\n"
            "e            - toggle erase mode\n"
            "s            - paint current block\n"
            "d            - erase current block\n"
            "q/w          - prev/next block type\n"
            "t            - toggle plane/half block/block\n"
            "\n"
            "m            - update minimap\n"
            "\n"
            "5            - save\n"
            ;

        g->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        g->drawString(helpText.c_str(), 40, 40);

        g->setDepthTest(true);
    }
    else
    {
        m_minimap->render();

        // render stats
        char str[128];
        sprintf(str, "Mode=%s Loc=[%d,%d,%d] Block=%d", getEditModeName().c_str(), m_cursor.m_x, m_cursor.m_y, m_cursor.m_z, m_cursorType);
        g->setColor(1.0, 1.0, 1.0, 1.0);
        g->drawString(str, 20, 20);
    }

    g->setPerspectiveProjection();    
}

void Editor::renderActiveBlock()
{
    Graphics* g = Context::get()->getGraphics();
    g->setColor(1, 1, 1, 1);

    int timer = (int)Utils::getCurrentTime();
    if (timer % 300 < 150)
    {
        g->pushMatrix();
        // offset cursor block slightly so it actually shows
        g->translate(0, 0, 0.1f);
        Block b = Block(m_cursorType, m_cursor);
        b.setRepresentation(m_cursorRepresentation);
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
    // road gets painted before we move the cursor
    if (m_editMode == EDITMODE_ROAD)
    {
        TexManager* tex = Context::get()->getTexManager();
        Tilemap* tilemap = tex->getTilemap();

        std::string newId = "";
        
        Block* bLeft = m_world->getBlockAt(m_cursor-Vec3i(1, 0, 0));
        Block* bRight = m_world->getBlockAt(m_cursor+Vec3i(1, 0, 0));
        Block* bUp = m_world->getBlockAt(m_cursor+Vec3i(0, 1, 0));
        Block* bDown = m_world->getBlockAt(m_cursor-Vec3i(0, 1, 0));

        std::string idLeft = tilemap->getTextureId(bLeft->getType());
        std::string idRight = tilemap->getTextureId(bRight->getType());
        std::string idUp = tilemap->getTextureId(bUp->getType());
        std::string idDown = tilemap->getTextureId(bDown->getType());

        printf("left=%s, right=%s, up=%s, down=%s\n", idLeft.c_str(), idRight.c_str(), idUp.c_str(), idDown.c_str());

        if (d.m_x != 0) 
        {
            newId = "road_horiz";

            if (idLeft == "road_vert")
            {
                bLeft->setType(tilemap->getTileIndex("road_L90"));
                m_world->insertBlock(bLeft);
            }
            if (idRight == "road_vert")
            {
                bRight->setType(tilemap->getTileIndex("road_L180"));
                m_world->insertBlock(bRight);
            }
        }
        else if (d.m_y != 0) 
        {
            newId = "road_vert";
        }
        
        if (newId != "")
        {
            int id = tilemap->getTileIndex(newId);
            printf("%s -> %d\n", newId.c_str(), id);
            Block* newRoad = new Block(id, m_cursor);
            m_world->insertBlock(newRoad);
        }
    }

    m_cursor = m_cursor + d;
    // TODO Vec3ui means we never go <0, but rather get -1>NU_MAX_LAYERS which makes layer switching wrap around
    if (m_cursor.m_z < 0)
        m_cursor.m_z = 0;
    if (m_cursor.m_z >= NU_MAX_LAYERS)
        m_cursor.m_z = NU_MAX_LAYERS-1;

    updateCameraPosition();

    if (m_editMode == EDITMODE_PAINT)
        paintCurrentBlock();

    if (m_editMode == EDITMODE_ERASE)
        eraseCurrentBlock();

}

void Editor::moveCamera(Vec3 d)
{
    m_cameraOffset = m_cameraOffset + d;

    updateCameraPosition();
}

void Editor::updateCameraPosition()
{
    Vec3 cameraLoc((float)m_cursor.m_x, (float)m_cursor.m_y, (float)m_cursor.m_z);
    cameraLoc = cameraLoc + m_cameraOffset;
    m_camera->moveTo(cameraLoc);
}

void Editor::changeCullDistanceBy(float d)
{
    float current = m_camera->getCullDistance();
    m_camera->setCullDistance(current+d);
}

void Editor::changeActiveBlockBy(char delta)
{
    TexManager* tex = Context::get()->getTexManager();
    Tilemap* tilemap = tex->getTilemap();

    m_cursorType = tilemap->getNextTile(m_cursorType, delta);
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
    if (key == NU_KEY_PAGE_UP) moveCamera(Vec3(0, 0, 5));
    if (key == NU_KEY_PAGE_DOWN) moveCamera(Vec3(0, 0, -5));

    // cull distance
    if (key == NU_KEY_HOME) changeCullDistanceBy(-1);
    if (key == NU_KEY_END) changeCullDistanceBy(1);

    // misc
    if (key == 'h') m_helpActive = !m_helpActive;
    
    // painting blocks
    if (key == 'p') changeEditMode(EDITMODE_PAINT);
    if (key == 'e') changeEditMode(EDITMODE_ERASE);
    if (key == 'r') changeEditMode(EDITMODE_ROAD);
    if (key == 's') paintCurrentBlock();
    if (key == 'd') eraseCurrentBlock();

    // minimap
    if (key == 'm') m_minimap->update();
    
    // change block type
    if (key == 'q') changeActiveBlockBy(-1);
    if (key == 'w') changeActiveBlockBy(1);
    if (key == 't') cycleCursorRepresentation();
    //if (key >= '1' && key <= '9') m_activeBlock = getBlocksetStart(key-'1');

    // saving & loading
    // TODO change to something that doesn't overlap with "blockset" selection
    if (key == '5') saveWorld();
}

void Editor::handleMouse()
{
    Mouse* mouse = Context::get()->getMouse();

    Mouse::KeyPress key = mouse->getKeyPress(Mouse::NU_MOUSE_LEFT);
    if (key.isDown)
    {
        // Minimap
        int x, y, width, height;
        m_minimap->getScreenLocation(x, y, width, height);
        
        if (key.x >= x && key.x < x+width &&
            key.y >= y && key.y < y+height)
        {
            Vec2i min;
            Vec2i max;
            m_minimap->getWorldMinMax(min, max);

            Vec2i coord;
            coord.m_x = min.m_x + ((max.m_x - min.m_x) * (key.x - x)) / width;
            coord.m_y = min.m_y + ((max.m_y - min.m_y) * (key.y - y)) / height;

            m_cursor.m_x = coord.m_x;
            m_cursor.m_y = coord.m_y;

            updateCameraPosition();
        }
    }
}

void Editor::cycleCursorRepresentation()
{
    m_cursorRepresentation++;
    if (m_cursorRepresentation >= Block::BLOCK_LASTREPRESENTATION)
        m_cursorRepresentation = 0;
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

    if (newMode == EDITMODE_ROAD)
    {
        m_editMode = (m_editMode == EDITMODE_ROAD) ? EDITMODE_NONE : EDITMODE_ROAD;
    }
}

void Editor::paintCurrentBlock()
{
    Block* block = new Block(m_cursorType, m_cursor);
    block->setRepresentation(m_cursorRepresentation);
    m_world->insertBlock(block);
    // TODO [muumi] I think this leaks memory. World calls Cell which calls "new Block" instead of using this.
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
    NU_UNREF(idx);
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
