#pragma once

#include <string>
#if defined(__APPLE__) || defined(MACOSX)
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

namespace Nultima
{

class Timer
{
public:
    Timer() {}
    ~Timer() {}

    void    start           (std::string name);
    void    stop            (std::string name);
    void    reset           (std::string name);
    double  getTimerValue   (std::string name);

private:
    struct InternalTimer
    {
        InternalTimer() : start(0LL), elapsed(0LL) {}
        double start;
        double elapsed;
    };

    std::tr1::unordered_map<std::string, InternalTimer> m_timerMap;
};

class ScopedTimer
{
public:
    ScopedTimer(std::string name, bool reset=true);
    ~ScopedTimer();

private:
    std::string m_name;
};

};
