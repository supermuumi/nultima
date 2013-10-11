#pragma once

#include "nuObject.h"
#include "nuVec3.h"

namespace Nultima
{

class Monster : public Object
{
public:
    Monster(Vec3i v);
    ~Monster();

    void  render() const;
    void  tick();

private:
    Vec3i m_position;
};

};
