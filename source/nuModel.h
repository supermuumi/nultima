#pragma once

#include "nuVec3.h"

#include <vector>

namespace Nultima
{

// TODO [sampo] reference count so that we can unload the models
class Model
{
public:

    enum Type
    {
        UNIT_PLANE = 0,
        UNIT_BOX,
        NUM_MODELS
    };

    Model (int type);
    ~Model() {};

    void render() const;

private:
    void createUnitPlane();
    void createUnitBox();

    void*   m_indexBuffer;
    void*   m_vertexBuffer;
    int     m_numTriangles;
};

}; // namespace
