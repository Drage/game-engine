
#include <SDL.h>
#include "Timer.h"

using namespace DrageEngine;

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::Start()
{
    if (paused)
    {
        paused = false;
        startTicks = SDL_GetPerformanceCounter() - pausedTicks;
        pausedTicks = 0;
    }
    else
    {
        started = true;
        startTicks = SDL_GetPerformanceCounter();
    }
}

void Timer::Stop()
{
    started = false;
    paused = false;
    pausedTicks = 0;
    startTicks = 0;
}

void Timer::Reset()
{
    started = true;
    paused = false;
    pausedTicks = 0;
    startTicks = SDL_GetPerformanceCounter();
}

void Timer::Pause()
{
    if (started && !paused)
    {
        paused = true;
        pausedTicks = SDL_GetPerformanceCounter() - startTicks;
    }
}

bool Timer::IsPaused() const
{
    return paused;
}

bool Timer::IsStarted() const
{
    return started;
}

float Timer::ElapsedSeconds() const
{
    if (started)
    {
        unsigned long ticks = paused ? pausedTicks : SDL_GetPerformanceCounter() - startTicks;
        return ticks / (float)SDL_GetPerformanceFrequency();
    }
    return 0;
}
