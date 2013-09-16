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

    typedef enum 
    {
        PLANE = 0,
        HALFBLOCK,
        BLOCK,

        BLOCK_LASTREPRESENTATION
    } BlockRepresentation;

    Block                       (void)      {}
    Block                       (char type, Vec3i location);
    ~Block                      (void)      {}

    void    serialize           (std::ofstream* stream);
    void    deserialize         (std::ifstream* stream);

    void    render              (void) const;

    void    moveTo              (Vec3i);
    void    setType             (char);
    char    getType             (void)      { return m_type; }
    void    setRepresentation   (NuUInt8);
    NuUInt8 getRepresentation   (void)      { return m_representation; }

    Vec3i   getLocation         (void) { return m_location; }

private:
    void    determineModel();

    char        m_type;
    Vec3i       m_location;
    NuUInt8     m_representation;
};

};
