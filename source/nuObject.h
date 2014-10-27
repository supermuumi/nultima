#pragma once

#include <stdio.h>
#include <fstream>

namespace Nultima
{

class Model;

class Object
{
public:

    Object() {}
    virtual ~Object() {}

    virtual void render     (void) const = 0;

protected:
    const Model* m_model;
};

}; // namespace
