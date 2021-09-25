
#include <SDL.h>
#include "Timer.h"

using namespace DrageEngine;

Timer::Timer()
{
    started = false;
    startTicks = 0;
}

void Timer::Start()
{
    started = true;
    startTicks = SDL_GetPerformanceCounter();
}

void Timer::Reset()
{
    startTicks = SDL_GetPerformanceCounter();
}

double Timer::ElapsedSeconds() const
{
    if (started)
    {
        unsigned long ticks = SDL_GetPerformanceCounter() - startTicks;
        return ticks / (double)SDL_GetPerformanceFrequency();
    }
    return 0;
}

double Timer::ElapsedMilliseconds() const
{
    return ElapsedSeconds() * 1000.0f;
}
