#include "nuWorld.h"
#include "nuMapLocation.h"

#include <fstream>

using namespace Nultima;

#define WORLD_VERSION_INITIAL 0

// TODO [sampo] streaming: unique tags for each block in the data 

World::World(std::string fileName)
{
    std::ifstream worldFile(fileName.c_str());
    if (!worldFile.bad())
    {
        // World format version
        NuInt32 version;
        worldFile >> version;

        // Player start
        MapLocation playerStart;
        worldFile >> playerStart;
    }
    worldFile.close();
}

