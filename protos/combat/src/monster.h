#include "vec3.h"

class Monster
{
public:
    enum Stage
    {
        UNDECIDED,
        WAITING,
        MOVING,
        MELEE_ATTACK
    };

    typedef struct
    {
        Monster::Stage stage;
        float duration;
    } Strategy;

    Monster(int x, int y);
    ~Monster() {};

    void render(bool isActive);
    void tick();

    bool isActive() { return (m_stage != UNDECIDED); }

private:
    int             m_x;
    int             m_y;
    Vec3            m_color;
    double          m_lastColorChange;
    int             m_stage;
    Strategy const* m_strategy;
    double          m_lastStageChange;
    int             m_strategyStep;
};