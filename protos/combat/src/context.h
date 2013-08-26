#include <vector>

class Player;
class Monster;

#define CONTEXT() Context::getContext()

class Context
{
public:
    Context() {};
    ~Context();

    static Context* getContext();
    static void     cleanup();

    void                    pushHero    (Player* hero)      { m_heroes.push_back(hero); }
    std::vector<Player*>    getHeroes   ()                  { return m_heroes; }

    void                    pushMonster (Monster* monster)  { m_monsters.push_back(monster); }
    std::vector<Monster*>   getMonsters ()                  { return m_monsters; }

private:
    std::vector<Player*>    m_heroes;
    std::vector<Monster*>   m_monsters;        
};
