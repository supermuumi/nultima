#include <time.h>
#include "nuUtils.h"

using namespace Nultima;

double Utils::getCurrentTime()
{
    return clock() / (CLOCKS_PER_SEC / 1000.0);
};
