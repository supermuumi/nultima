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
        ROCK,
        TYPE_LAST
    };

    typedef enum 
    {
        PLANE = 0,
        HALFBLOCK,
        BLOCK,

        BLOCK_LASTREPRESENTATION
    } BlockRepresentation;

    Block() {}
    Block(char type, Vec3i location);
    ~Block() {}

    void    serialize      (std::ofstream* stream);
    void    deserialize    (std::ifstream* stream);

    void    render         () const;

    void    moveTo(Vec3i);
    void    setType(char);
    char    getType() { return m_type; }
    void    setRepresentation(NuUInt8);

    Vec3i   getLocation() { return m_location; }

private:
    void    determineModel();

    char        m_type;
    Vec3i       m_location;
    std::string m_texName;
    NuUInt8     m_representation;
};

};
