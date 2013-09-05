#include <string>
#include <fstream>
#include <stdio.h>
#include "nuFileUtils.h"

using namespace Nultima;

char* FileUtils::readFile(std::string fname)
{
    char* buffer = NULL;
    std::ifstream file(fname.c_str(), std::ios::in);

    if (file.is_open())
    {
	file.seekg(0, std::ios::end);
	long size = file.tellg();
	printf("readFile(): size=%ld\n", size);
	buffer = new char[size];
	file.seekg(0, std::ios::beg);
	file.read(buffer, size);
	file.close();
    }

    return buffer;
}
