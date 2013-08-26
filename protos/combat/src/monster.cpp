#include <time.h>
#include <assert.h>
#include <vector>

#include "monster.h"
#include "gl_utils.h"
#include "context.h"

static const Vec3 s_defaultColor = Vec3(0.5, 0.2, 0);
static const Vec3 s_blinkColor = Vec3(0.6, 0.6, 0.6);

const Monster::Strategy g_approach[] =
{
    {Monster::WAITING, 1000},
    {Monster::MOVING, -1},
    {Monster::WAITING, 1000}
};

Monster::Monster(int x, int y) :
    m_x(x),
    m_y(y),
    m_color(s_defaultColor),
    m_lastColorChange(0),
    m_stage(UNDECIDED),
    m_strategy(NULL),
    m_strategyStep(0)
{}

typedef struct
{
    int x;
    int y;
} Move;

const Move s_possibleMoves[] =
{
    {0, -1},
    {1, 0},
    {-1, 0},
    {0, 1},
};

// TODO approach player instead of move randomly
void Monster::move()
{
    int newX = m_x;
    int newY = m_y;

    for (int i=0; i<sizeof(s_possibleMoves)/sizeof(s_possibleMoves[0]); i++)
    {
        newX = m_x + s_possibleMoves[i].x;
        newY = m_y + s_possibleMoves[i].y;

        // Collide with other monsters
        bool collision = false;
        std::vector<Monster*> monsters = CONTEXT()->getMonsters();
        for (std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)
            if (newX == (*it)->m_x && newY == (*it)->m_y)
                collision = true;

        if (!collision)
            break;
    }

    // take step
    m_x = newX;
    m_y = newY;
}

void Monster::tick()
{
    double timer = clock() / (CLOCKS_PER_SEC / 1000.0);

    // Make a decision
    if (m_stage == UNDECIDED)
    {
        // If next to a player, attack (if many which one?)
        // If not, move
        m_strategy = g_approach;

        m_strategyStep = 0;
        m_lastStageChange = timer;
        m_stage = m_strategy[m_strategyStep].stage;
    }

    // Do action
    switch (m_strategy[m_strategyStep].stage)
    {
    case UNDECIDED:
        assert(0);
    
    case WAITING:
        break;

    case MOVING:
        // TODO [sampo] approach the closest player:
        move();
        break;

    case MELEE_ATTACK:
        // TODO [sampo] attack an adjacent player
        break;
    }

    // Move to the next step
    if (timer - m_lastStageChange > m_strategy[m_strategyStep].duration)
    {
        m_lastStageChange = timer;
        m_strategyStep++;
        m_stage = m_strategy[m_strategyStep].stage;

        // Done
        if (m_strategyStep == sizeof(m_strategy) / sizeof(m_strategy[0]))
            m_stage = UNDECIDED;
    }
}

void Monster::render(bool isActive)
{
    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    double timer = clock() / (CLOCKS_PER_SEC / 1000.0);
    if (isActive && timer - m_lastColorChange >= 300.f)
    {
        m_lastColorChange = timer;
        m_color = (m_color == s_defaultColor) ? s_blinkColor : s_defaultColor;
    }

    if (!isActive)
        m_color = s_defaultColor;

    glPushMatrix();
    glColor3f(m_color.x, m_color.y, m_color.z);
    cubeAt((float)m_x, (float)m_y, 0.5f);
    glPopMatrix();

    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
}