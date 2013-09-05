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
    ~Object() {}

    virtual void render     (void) const = 0;
    virtual void serialize  (std::ofstream* stream) = 0;
    virtual void deserialize(std::ifstream* stream) = 0;

protected:
    const Model* m_model;
};

}; // namespace
