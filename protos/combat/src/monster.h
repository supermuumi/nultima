#include "vec3.h"

typedef struct
{
    int x;
    int y;
} Vec2;

class Player;

class Monster
{
public:
    enum Stage
    {
        UNDECIDED,
        WAITING,
        MOVING,
        MELEE_ATTACK,
        END
    };

    typedef struct
    {
        Monster::Stage stage;
        float duration;
    } Strategy;

    Monster(Vec2 position);
    ~Monster() {};

    void render(bool isActive);
    void tick();
    void prepare();
    void move();
    void attack();

    bool isActive() { return (m_stage != END); }
    bool isAlive() { return m_hp > 0; }
    Vec2 getPosition() { return m_position; }

private:
    Player* findClosestPlayer(int& distance);

    Vec2            m_position;
    Vec3            m_color;
    double          m_lastColorChange;
    int             m_stage;
    Strategy const* m_strategy;
    double          m_lastStageChange;
    int             m_strategyStep;
    int             m_hp;
    Player*         m_targetPlayer;
};