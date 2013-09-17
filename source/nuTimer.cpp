#include "nuTimer.h"
#include "nuContext.h"
#include "nuUtils.h"

using namespace Nultima;

void Timer::start(std::string name)
{
    m_timerMap[name].start = Utils::getCurrentTime();
}

void Timer::stop(std::string name)
{
    m_timerMap[name].elapsed += Utils::getCurrentTime() - m_timerMap[name].start;
    m_timerMap[name].start = 0;
}

void Timer::reset(std::string name)
{
    m_timerMap[name].elapsed = 0;
}

double Timer::getTimerValue(std::string name)
{
    return m_timerMap[name].elapsed;
}

ScopedTimer::ScopedTimer(std::string name, bool reset)
{
    Timer* timer = Context::get()->getTimer();
    if (reset)
        timer->reset(name);
    timer->start(name);

    m_name = name;
}

ScopedTimer::~ScopedTimer()
{
    Timer* timer = Context::get()->getTimer();
    timer->stop(m_name);
}
