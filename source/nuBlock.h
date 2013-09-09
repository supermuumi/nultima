#pragma once

#include "nuDefs.h"
#include "nuObject.h"
#include "nuVec3.h"

#include <fstream>

namespace Nultima
{

class Block : public Object
{
public:

    enum Type
    {
        GRASS = 0,
        WATER,
        ROCK
    };

    typedef enum 
    {
        AUTO = 0,
        PLANE,
        BLOCK
    } BlockRepresentation;

    Block() {}
    Block(char type, Vec3i location);
    ~Block() {}

    void serialize      (std::ofstream* stream);
    void deserialize    (std::ifstream* stream);

    void render         () const;

    void moveTo(Vec3i);
    void setType(char);
    void setRepresentation(NuUInt8);

    Vec3i   getLocation() { return m_location; }

private:
    void determineModel     ();
//    void determineTexName   ();

    char        m_type;
    Vec3i       m_location;
    std::string m_texName;
    NuUInt8     m_representation;
};

};
