#include "nuMinimap.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuObject.h"
#include "nuUiQuad.h"
#include "nuWorld.h"
#include "nuCell.h"
#include "nuDefs.h"
#include "nuTexManager.h"

#define MINIMAP_WIDTH   256
#define MINIMAP_HEIGHT  128

using namespace Nultima;

static const unsigned int s_backgroundColor = 0xffb469ff;

Minimap::Minimap(World* world)
{
    Graphics* g = Context::get()->getGraphics();
    m_pixels.resize(MINIMAP_WIDTH*MINIMAP_HEIGHT, s_backgroundColor);
    m_texture = g->generateTexture((unsigned char*)&m_pixels[0], MINIMAP_WIDTH, MINIMAP_HEIGHT);
    m_quad = new UiQuad(0, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT, m_texture);

    m_world = world;
}

Minimap::~Minimap()
{
    delete m_quad;
}

void Minimap::render()
{
    Graphics* g = Context::get()->getGraphics();
    g->setTextureData(m_texture, (unsigned char*)&m_pixels[0], 0, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT);
    g->bindTexture(m_texture);
    m_quad->render();
}

unsigned int Minimap::determineColor(Vec2i coord, int size)
{
    TexManager* tex = Context::get()->getTexManager();
    std::vector<int> numTypes;
    numTypes.resize(tex->getNumTiles(), 0);
    
    int x = coord.m_x;
    int y = coord.m_y;

    for (int j=y; j<y+size; j++)
    for (int i=x; i<x+size; i++)
    for (int k = NU_MAX_LAYERS-1; k>=0; k--)
    {
        Vec3i location = Vec3i(x, y, k);
        Block* block = m_world->getBlockAt(location);
        if (block)
        {
            char type = block->getType();
            numTypes[type] += 1;
        }
    }

    int mostFrequentType = -1;
    int mostFrequentCount = 0;
    for (int i=0; i<tex->getNumTiles(); i++)
        if (numTypes[i] > mostFrequentCount)
            mostFrequentType = i;

    unsigned int color = s_backgroundColor;

    if (mostFrequentType != -1)
    {
        Vec3ui c = tex->getTilemap()->getTileColor(mostFrequentType);
        color = (c.m_x & 0xff) +
                ((c.m_y & 0xff) << 8) +
                ((c.m_z & 0xff) << 16) +
                (0xff << 24);
    }

    return color;
}

void Minimap::getScreenLocation(int& x, int& y, int& width, int& height)
{
    x = 0;
    y = 0;
    width = MINIMAP_WIDTH;
    height = MINIMAP_HEIGHT;
}

void Minimap::update()
{
    m_pixels.resize(MINIMAP_WIDTH*MINIMAP_HEIGHT, s_backgroundColor);

    Vec2i min = m_world->getMinCoordinate();
    Vec2i max = m_world->getMaxCoordinate();
    Vec2i dim = max-min;

    if (dim.m_x < MINIMAP_WIDTH)
        min.m_x -= MINIMAP_WIDTH - dim.m_x;

    if (dim.m_y < MINIMAP_HEIGHT)
        min.m_y -= MINIMAP_HEIGHT - dim.m_y;

    dim = max-min;

    Vec2 blocks;
    blocks.m_x = (float)dim.m_x / (float)MINIMAP_WIDTH;
    blocks.m_y = (float)dim.m_y / (float)MINIMAP_HEIGHT;

    Vec2i blocksi = Vec2i((int)std::ceil(blocks.m_x), (int)std::ceil(blocks.m_y));;
    int maxBlocks = (blocksi.m_x > blocksi.m_y) ? blocksi.m_x : blocksi.m_y;

    Vec2i current = min;
    for (int i=0; i<MINIMAP_WIDTH; i++)
    for (int j=0; j<MINIMAP_HEIGHT; j++)
    {
        Vec2i current = Vec2i(min.m_x+i*maxBlocks, min.m_y+j*maxBlocks); 
        m_pixels[i+(MINIMAP_HEIGHT-1-j)*MINIMAP_WIDTH] = determineColor(current, maxBlocks);
    }
}

void Minimap::getWorldMinMax(Vec2i& min, Vec2i& max)
{
    min = m_world->getMinCoordinate();
    max = m_world->getMaxCoordinate();
    
    Vec2i dim = max-min;

    if (dim.m_x < MINIMAP_WIDTH)
        min.m_x -= MINIMAP_WIDTH - dim.m_x;

    if (dim.m_y < MINIMAP_HEIGHT)
        min.m_y -= MINIMAP_HEIGHT - dim.m_y;

    int multX = (int)std::ceil((float)dim.m_x / (float)MINIMAP_WIDTH);
    int multY = (int)std::ceil((float)dim.m_y / (float)MINIMAP_HEIGHT);
    max.m_x += multX*MINIMAP_WIDTH - dim.m_x;
    max.m_y += multY*MINIMAP_HEIGHT - dim.m_y;

    if (multY > multX)
        max.m_x += (multY-multX)*MINIMAP_WIDTH;
    else
        max.m_y += (multX-multY)*MINIMAP_HEIGHT;
}
