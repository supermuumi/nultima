#include <stdio.h>

#include <string>
#include <fstream>
#include "nuFileUtils.h"
#include "document.h"

using namespace Nultima;

char* FileUtils::readFile(std::string fname)
{
    char* buffer = NULL;
	FILE* fp;
	
	fp = fopen(fname.c_str(), "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buffer = new char[size+1];

		long bytesLeftToRead = size;
		while (bytesLeftToRead > 0) {
			int bytesRead = fread(buffer, 1, bytesLeftToRead, fp);
			bytesLeftToRead -= bytesRead;
		}
		fclose(fp);
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
