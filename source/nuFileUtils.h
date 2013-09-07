#pragma once

#include <string>
#include "document.h"

namespace Nultima
{

class FileUtils
{

public:
    static char*               readFile(std::string fname);
    static rapidjson::Document readJSON(std::string fname);
};


}; // namespace
