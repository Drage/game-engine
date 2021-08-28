
#include <SDL.h>
#include "Timer.h"

using namespace DrageEngine;

Timer::Timer()
{
    m_startTicks = 0;
    m_pausedTicks = 0;
    m_paused = false;
    m_started = false;
}

void Timer::Start()
{
    if (m_paused)
    {
        m_paused = false;
        m_startTicks = SDL_GetPerformanceCounter() - m_pausedTicks;
        m_pausedTicks = 0;
    }
    else
    {
        m_started = true;
        m_startTicks = SDL_GetPerformanceCounter();
    }
}

void Timer::Stop()
{
    m_started = false;
    m_paused = false;
    m_pausedTicks = 0;
    m_startTicks = 0;
}

void Timer::Reset()
{
    m_started = true;
    m_paused = false;
    m_pausedTicks = 0;
    m_startTicks = SDL_GetPerformanceCounter();
}

void Timer::Pause()
{
    if (m_started && !m_paused)
    {
        m_paused = true;
        m_pausedTicks = SDL_GetPerformanceCounter() - m_startTicks;
    }
}

bool Timer::IsPaused() const
{
    return m_paused;
}

bool Timer::IsStarted() const
{
    return m_started;
}

float Timer::ElapsedSeconds() const
{
    if (m_started)
    {
        unsigned long ticks = m_paused ? m_pausedTicks : SDL_GetPerformanceCounter() - m_startTicks;
        return ticks / (float)SDL_GetPerformanceFrequency();
    }
    return 0;
}
