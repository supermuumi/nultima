#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include "libjson.h"

std::vector<std::string> g_lines;
std::map<std::string, int> g_keywords;

char* readFile(const char *filename)
{
    char* buffer = 0;
    long length;
    FILE* f = fopen(filename, "rb");

    if (f)
    {
	fseek (f, 0, SEEK_END);
	length = ftell(f);
	fseek (f, 0, SEEK_SET);
	buffer = new char [length+1];
	fread(buffer, 1, length, f);
	buffer[length] = 0;
	fclose(f);
    }

    return buffer;
}

void parseLines(const JSONNode& n) 
{
    JSONNode::const_iterator it = n.begin();
    while (it != n.end()) 
    {
	g_lines.push_back(it->as_string());
	++it;
    }
}

void parseKeywords(const JSONNode& n)
{
    JSONNode::const_iterator it = n.begin();
    while (it != n.end()) 
    {
	// stupid workaround, for some reason it->as_int() doesn't link!
	g_keywords[it->name()] = atoi((it->as_string()).c_str());
	++it;
    }
}

void ParseJSON(const JSONNode& n)
{
    JSONNode::const_iterator it = n.begin();
    while (it != n.end()) 
    {
	std::string nodeName = it->name();
	if (nodeName == "lines")
	    parseLines(*it);
	else if (nodeName == "keywords")
	    parseKeywords(*it);

	//increment the iterator
	++it;
    }
}

int main(void)
{
    char* jsonData = readFile("test.json");

    JSONNode dialogue = libjson::parse(jsonData);
    ParseJSON(dialogue);

    // start with a greeting
    std::cout << g_lines.at(0) << std::endl;
    // conversation loop
    while (true) 
    {
	std::string cmd;

	std::cout << "You say: ";
	std::cin >> cmd;
	
	if (cmd == "bye")
	    break;

	std::map<std::string, int>::iterator it = g_keywords.find(cmd);
	if (it == g_keywords.end())
	    std::cout << "Whatchoo talkin' about Willis?" << std::endl;
	else 
	    std::cout << g_lines.at(it->second) << std::endl;
    }

    return 0;
}
