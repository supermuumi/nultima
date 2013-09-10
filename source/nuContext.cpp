#include "nuContext.h"
#include "nuGraphics.h"
#include "nuDefs.h"
#include "nuKeyboard.h"
#include "nuModel.h"
#include "nuTexManager.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace Nultima;

Context* g_context = NULL;

Context::Context() :
    m_argc(0),
    m_argv(NULL),
    m_graphics(NULL),
    m_keyboard(NULL)
{}

Context::~Context()
{
    NU_ASSERT(!m_graphics);
    NU_ASSERT(!m_keyboard);
}

Context* Context::get()
{
    if (!g_context)
        g_context = new Context();

    return g_context;
}

void Context::init(int argc, char** argv)
{
    m_argc = argc;
    m_argv = argv;

    m_graphics = new Graphics(Vec2i(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_graphics->init(argc, argv);

    m_keyboard = new Keyboard();

    m_models.resize(Model::NUM_MODELS, NULL);

    m_texManager = new TexManager();
}

void Context::deinit()
{
    m_graphics->deinit();
    delete m_graphics;
    m_graphics = NULL;

    delete m_keyboard;
    m_keyboard = NULL;

    delete m_texManager;
    m_texManager = NULL;
}

void Context::release()
{
    delete g_context;
    g_context = NULL;
}

std::string Context::getWorldFile()
{
    // TODO [sampo] parse from output? read from json?
    return "../../assets/maps/kallio.world";
}

std::string Context::getStateFile()
{
    return "";
}

const Model* Context::getModel(int type)
{
    // lazy create
    if (!m_models[type])
        m_models[type] = new Model(type);

    return m_models[type];
}

TexManager* Context::getTexManager()
{
    return m_texManager;
}

unsigned int Context::getTexture(std::string name)
{
    return m_texManager->getTexture(name);
}

Tilemap::TilemapTexture Context::getTilemapTexture(std::string id)
{
    return m_texManager->getTilemapTexture(id);
}
