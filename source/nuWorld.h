#pragma once

#include "nuMapLocation.h"
#include "nuCell.h"

#include <string>
// TODO - tr1 seems to be required on osx at least
#if defined(__APPLE__) || defined(MACOSX)
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

namespace Nultima
{

class World
{
public:
    World(std::string fileName);
    ~World() {}

    bool getPlayerStart(MapLocation& location) { (void)location; return true; }
    std::vector<Cell*> getCells() { return m_loadedCells; }

private:
    std::unordered_map<unsigned int, Cell*> m_cellMap;
    std::vector<Cell*>                      m_loadedCells;
};

}; // namespace
