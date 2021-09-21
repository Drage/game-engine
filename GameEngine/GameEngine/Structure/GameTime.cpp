
#include <SDL.h>
#include <iostream>
#include "GameTime.h"
#include "Application.h"
#include "Debug.h"

using namespace DrageEngine;

float Time::deltaTime = 0;
float Time::timeScale = 1.0f;
unsigned long Time::startTime = 0;
unsigned long Time::currentTime = 0;
unsigned long Time::tickFrequency;

void Time::Init()
{
    tickFrequency = SDL_GetPerformanceFrequency();
    startTime = SDL_GetPerformanceCounter();
    currentTime = startTime;
}

void Time::Update()
{
    unsigned long previousFrameTime = currentTime;
    currentTime = SDL_GetPerformanceCounter();
    deltaTime = (currentTime - previousFrameTime) / (float)tickFrequency;
    
    if (app->window->VSync())
        deltaTime = 1.0f / app->window->GetRefreshRate();
}

float Time::RunTime()
{
    return (currentTime - startTime) / (float)tickFrequency;
}

float Time::DeltaTime()
{
    return deltaTime * timeScale;
}

float Time::TimeScale()
{
    return timeScale;
}

void Time::SetTimeScale(float timeScale)
{
    Time::timeScale = timeScale;
}

float Time::UnscaledDeltaTime()
{
    return deltaTime;
}
