#pragma once

#include "nuVec3.h"
#include "nuVec2.h"

#include <vector>

namespace Nultima
{

// TODO [sampo] reference count so that we can unload the models
// TODO [sampo] ModelFactory for the Model Types, Model should only take raw vertex data
class Model
{
public:

    enum Type
    {
        UNIT_PLANE = 0,
        UNIT_BOX,
        HALFBOX,
        TORUS,
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
    void createUnitHalfBox();
    void createTorus(int r, int p, float radius, float thickness);
    
    unsigned int m_indexBuffer;
    unsigned int m_vertexBuffer;
    int          m_numTriangles;
};

}; // namespace
