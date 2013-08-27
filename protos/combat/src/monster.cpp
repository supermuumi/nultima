#include <time.h>
#include <assert.h>
#include <vector>
#include <stdlib.h>

#include "monster.h"
#include "gl_utils.h"
#include "context.h"
#include "Player.h"

static const Vec3 s_defaultColor = Vec3(0.5, 0.2, 0);
static const Vec3 s_blinkColor = Vec3(0.6, 0.6, 0.6);
static const Vec3 s_deadColor = Vec3(0, 0, 0);

const Monster::Strategy g_approach[] =
{
    {Monster::WAITING, 300},
    {Monster::MOVING, -1},
    {Monster::WAITING, 300},
    {Monster::END, -1}
};

const Monster::Strategy g_attack[] =
{
    {Monster::WAITING, 1000},
    {Monster::MELEE_ATTACK, -1},
    {Monster::WAITING, 1000},
    {Monster::END, -1}
};

Monster::Monster(Vec2 position) :
    m_position(position),
    m_color(s_defaultColor),
    m_lastColorChange(0),
    m_stage(UNDECIDED),
    m_strategy(NULL),
    m_strategyStep(0),
    m_hp(3),
    m_targetPlayer(NULL)
{
}

const Vec2 s_adjacentTiles[] =
{
    {0, -1},
    {1, 0},
    {-1, 0},
    {0, 1},
};

// TODO approach player instead of move randomly
void Monster::move()
{
    int newX = m_position.x;
    int newY = m_position.y;

    int dX = m_targetPlayer->m_x-m_position.x;
    int dY = m_targetPlayer->m_y-m_position.y;
    if (abs(dX) >= abs(dY))
    {
        if (dX > 0)
            newX++;
        else
            newX--;
    }
    else
    {
        if (dY > 0)
            newY++;
        else
            newY--;
    }

    // Collide with other monsters
    bool collision = false;
    std::vector<Monster*> monsters = CONTEXT()->getMonsters();
    for (std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)
    {
        if (!(*it)->isAlive())
            continue;

        if (newX == (*it)->m_position.x && newY == (*it)->m_position.y)
            collision = true;
    }

    if (!collision)
    {
        // take step
        m_position.x = newX;
        m_position.y = newY;
    }
    else
    {
        printf("  blocked!\n");
    }
}

void Monster::attack()
{
    m_hp--;
    printf(" Monster hp's remaining %d/3\n", m_hp);
}

Player* Monster::findClosestPlayer(int& distance)
{
    Player* ret = NULL;
    distance = 1000;

    std::vector<Player*> players = CONTEXT()->getHeroes();
    for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); ++it)
    {
        if (!(*it)->isAlive())
            continue;

        int d = abs((*it)->m_x - m_position.x) + abs((*it)->m_y - m_position.y);
        
        if (d < distance)
        {
            distance = d;
            ret = *it;
        }
    }
    return ret;
}

void Monster::prepare()
{
    m_stage = UNDECIDED;
}

void Monster::tick()
{
    if (m_hp <= 0)
        return;

    double timer = clock() / (CLOCKS_PER_SEC / 1000.0);

    // Make a decision
    if (m_stage == UNDECIDED)
    {
        m_targetPlayer = NULL;

        int distance = 0;
        m_targetPlayer = findClosestPlayer(distance);

        // Monsters won!
        if (m_targetPlayer == NULL)
            return;

        // If next to a player, attack (if many which one
        if (distance == 1)
            m_strategy = g_attack;

        // If not, move
        else
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
        printf(" moving\n");
        // TODO [sampo] approach the closest player:
        move();
        break;

    case MELEE_ATTACK:
        assert(m_targetPlayer != NULL);
        m_targetPlayer->attack();
        break;

    case END:
        printf(" end\n");
        break;

    default:
        assert(0);
    }

    // Move to the next step
    if (timer - m_lastStageChange > m_strategy[m_strategyStep].duration)
    {
        m_lastStageChange = timer;
        m_strategyStep++;
        m_stage = m_strategy[m_strategyStep].stage;
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

    if (m_hp <= 0)
        m_color = s_deadColor;

    glPushMatrix();
    glColor3f(m_color.x, m_color.y, m_color.z);
    cubeAt((float)m_position.x, (float)m_position.y, 0.5f);
    glPopMatrix();

    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
}