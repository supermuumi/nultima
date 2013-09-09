#include <string>
#include <fstream>
#include "nuFileUtils.h"
#include "document.h"

using namespace Nultima;

char* FileUtils::readFile(std::string fname)
{
    char* buffer = NULL;
    std::ifstream file(fname.c_str(), std::ios::in);

    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        long size = (long)file.tellg();
        buffer = new char[size+1];
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();
		buffer[size] = 0;
    }

    return buffer;
}

rapidjson::Document FileUtils::readJSON(std::string fname)
{
    rapidjson::Document doc;
    char* jsonBlob;

    jsonBlob = FileUtils::readFile(fname.c_str());
    assert(jsonBlob != NULL);
    doc.Parse<0>(jsonBlob);
    delete jsonBlob;

    return doc;
}
