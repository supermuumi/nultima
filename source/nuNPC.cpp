#include <string>
#include "nuNPC.h"
#include "nuModel.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuUtils.h"
#include "nuFileUtils.h"
#include "nuLog.h"
#include "rapidjson/document.h"

using namespace Nultima;

NPC::NPC(std::string fname)
{
    rapidjson::Document doc;
    // TODO proper path
    char* jsonBlob = FileUtils::readFile(std::string("../../assets/npc/"+fname+".json"));
    assert(jsonBlob != NULL);
    doc.Parse<0>(jsonBlob);
    delete jsonBlob;
    assert(!doc.HasParseError());

    // generic stats
    m_name     = doc["stats"]["name"].GetString();
    m_canJoin  = doc["stats"]["canJoin"].GetBool();

    // waypoints
    const rapidjson::Value& waypoints = doc["waypoints"]; // Using a reference for consecutive access is handy and faster.
    for (rapidjson::SizeType i = 0; i < waypoints.Size(); i++)
    {
        const rapidjson::Value& data = waypoints[i];
        NPCWaypoint wp = {Vec3i(data[0u].GetInt(), data[1].GetInt(), data[2].GetInt()), data[3].GetInt()};
        m_waypoints.push_back(wp);
    }
    m_curWaypoint = 0;
    m_waypointTurnsLeft = m_waypoints[0].numTurns;
    m_position = m_waypoints[0].pos;
    m_model    = new Model(Model::UNIT_BOX);

}

NPC::~NPC()
{
    delete m_model;
}

void NPC::tick()
{
    // TODO animate NPCs who are visible
}

void NPC::processTurn()
{
    m_waypointTurnsLeft--;
    if (m_waypointTurnsLeft == 0)
    {
        m_curWaypoint++;
        // TODO uhh why the fuck can't i use m_waypoints.size() in the if below on matter if m_curWaypoint is unsigned or not
        int x = m_waypoints.size();
        if (m_curWaypoint >= x)
            m_curWaypoint = 0;

        // TODO [muumi] just a temp hack, add A* or something for pathfinding between waypoints
        m_position = m_waypoints[m_curWaypoint].pos;
        m_waypointTurnsLeft = m_waypoints[m_curWaypoint].numTurns;
    }
}

void NPC::render() const
{
    Graphics* g = Context::get()->getGraphics();
    g->setColor(1, 1, 0, 1);
    g->setTexturing(false);
    g->pushMatrix();
    g->translate(m_position);
    m_model->render();
    g->popMatrix();
    g->setTexturing(true);
}
