
#include <SDL.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
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

std::string Time::Now()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto time = system_clock::to_time_t(now);
    std::tm local = *std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(&local, "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}
