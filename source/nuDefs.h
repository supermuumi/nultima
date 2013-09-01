#pragma once

#include <stddef.h>
#include <assert.h>

#define NU_CELL_WIDTH   16
#define NU_CELL_HEIGHT  16

#define NU_UNREF(X) (void)X

#if defined(_DEBUG)
#   undef NU_DEBUG
#   define NU_DEBUG
#endif

#if defined(NU_DEBUG)
#   define NU_ASSERT(X) assert(X)
#else
#   define NU_ASSERT(X)
#endif

#define NU_OS_OSX 0
#define NU_OS_WINDOWS 1

#if defined(__APPLE__)
#   define NU_OS NU_OS_OSX
#elif defined(_WIN32)
#   define NU_OS NU_OS_WINDOWS
#else
#   error Platofrm not supported!
#endif

namespace Nultima
{

typedef unsigned char   NuUInt8;
typedef signed char     NuInt8;
typedef unsigned int    NuUInt32;
typedef int             NuInt32;
};
