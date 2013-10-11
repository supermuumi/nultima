#include "nuMonster.h"
#include "nuModel.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuUtils.h"

using namespace Nultima;

Monster::Monster(Vec3i v)
{
    m_model = new Model(Model::UNIT_BOX);
    m_position = v;
}

Monster::~Monster()
{
    delete m_model;
}

void Monster::tick()
{
}

void Monster::render() const
{
    Graphics* g = Context::get()->getGraphics();
    g->setColor(1, 0, 0, 1);
    g->setTexturing(false);
    g->pushMatrix();
    g->translate(m_position);
    m_model->render();
    g->popMatrix();
    g->setTexturing(true);
}
