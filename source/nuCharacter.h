#pragma once

#include "nuObject.h"

namespace Nultima
{

class Character : public Object
{
public:
    Character();
    ~Character();

    void render() const;
    void tick();

private:
    float   m_rotateX;
    float   m_rotateY;
    double  m_lastTime;
};

};