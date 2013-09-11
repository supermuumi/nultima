#pragma once

#include "rapidjson/document.h"

#include <string>

namespace Nultima
{

class FileUtils
{

public:
    static char*               readFile(std::string fname);
    static rapidjson::Document readJSON(std::string fname);
};


}; // namespace
