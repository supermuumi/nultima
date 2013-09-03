#pragma once

#include <stdio.h>
namespace Nultima
{

class Object
{
public:

    Object() {}
    ~Object() {}

    virtual void render() const = 0;
};

}; // namespace
