
#ifndef TIME_H
#define TIME_H

#include <string>

namespace DrageEngine
{
    class Time
    {
        public:
            static float RunTime();
            static float DeltaTime();

            static float TimeScale();
            static void SetTimeScale(float timeScale);

            static float UnscaledDeltaTime();
        
            static std::string Now();

        private:
            friend class Application;
        
            static void Init();
            static void Update();
        
            static float deltaTime;
            static float timeScale;
            static unsigned long startTime;
            static unsigned long currentTime;
            static unsigned long tickFrequency;
    };
}

#endif
