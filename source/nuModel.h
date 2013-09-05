#pragma once

#include "nuVec3.h"
#include "nuVec2.h"

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
    typedef struct
    {
        Vec3 coords;
        Vec3 normal;
        Vec2 texCoord;
    } Vertex;

    void createUnitPlane();
    void createUnitBox();
    
    unsigned int m_indexBuffer;
    unsigned int m_vertexBuffer;
    int          m_numTriangles;
};

}; // namespace
