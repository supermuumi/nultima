#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "nuDefs.h"
#include "nuLog.h"

using namespace Nultima;

void Log::msg(const char* msg, ...)
{
#ifdef NU_DEBUG
    printf("[MSG] "); 

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
#endif
}

void Log::warn(const char* msg, ...)
{
#ifdef NU_DEBUG
    printf("[WARN] "); 

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
#endif
}

void Log::error(const char* msg, ...)
{
#ifdef NU_DEBUG
    printf("[ERROR] "); 

    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
#endif
}
