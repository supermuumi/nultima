#pragma once

namespace Nultima
{

class Log
{
public:
    static void msg(const char* msg, ...);
    static void warn(const char* msg, ...);
    static void error(const char* msg, ...);
};

};
