#pragma once

#include "nuDefs.h"
#include "nuObject.h"
#include "nuVec3.h"
#include <string>
#include <vector>

namespace Nultima
{

    class Vec3i;

class NPC : public Object
{
public:
    NPC(std::string fname);
    ~NPC();

    void  render() const;
    void  tick();
    void  processTurn();

private:
    Vec3i                    m_position;

    std::string              m_name;
    bool                     m_canJoin;

    typedef struct 
    {
        Vec3i pos;
        int   numTurns;
    } NPCWaypoint;


    std::vector<NPCWaypoint> m_waypoints;
    int                      m_curWaypoint;
    unsigned int             m_waypointTurnsLeft;

};

};
