#include "nuCharacter.h"
#include "nuModel.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuUtils.h"

using namespace Nultima;

#define X_ROTATE_PER_SEC 300.f
#define Y_ROTATE_PER_SEC 200.f

Character::Character()
{
    m_model = new Model(Model::TORUS);
    m_rotateX = 0.f;
    m_rotateY = 0.f;
    m_lastTime = Utils::getCurrentTime();
}

void Character::tick()
{
    // TODO [sampo] Store a single time for all actors to use
    // TODO [sampo]do updates in tick!!
    double time = Utils::getCurrentTime();
    double elapsed = time - m_lastTime;
    m_lastTime = time;

    m_rotateX += X_ROTATE_PER_SEC * ((float)elapsed / 1000);
    m_rotateY += Y_ROTATE_PER_SEC * ((float)elapsed / 1000);
}

void Character::render() const
{
    Graphics* g = Context::get()->getGraphics();
    g->setColor(1, 0, 0, 1);
    g->setTexturing(false);
    g->pushMatrix();
    g->rotate(m_rotateX, 1, 0, 0);
    g->rotate(m_rotateY, 0, 1, 0);
    m_model->render();
    g->popMatrix();
    g->setTexturing(true);
}
