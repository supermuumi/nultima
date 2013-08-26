#include "context.h"

Context* g_context = NULL;

Context::~Context()
{
    // delete players
    while (m_heroes.size())
    {
        Player* jope = m_heroes.back();
        delete jope;
        m_heroes.pop_back();
    }

    // delete monsters
    while (m_monsters.size())
    {
        Monster* monsu = m_monsters.back();
        delete monsu;
        m_monsters.pop_back();
    }
}

Context* Context::getContext()
{
    if (!g_context)
        g_context = new Context();

    return g_context;
}

void Context::cleanup()
{
    delete g_context;
}