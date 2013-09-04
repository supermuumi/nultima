#pragma once

#include <stdio.h>
namespace Nultima
{

class Model;

class Object
{
public:

    Object() {}
    ~Object() {}

    virtual void render() const = 0;

protected:
    const Model* m_model;
};

}; // namespace
