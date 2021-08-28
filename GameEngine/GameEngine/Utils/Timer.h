
#ifndef TIMER_H
#define TIMER_H

namespace DrageEngine
{
    class Timer
    {
        public:
            Timer();
            void Start();
            void Pause();
            void Stop();
            void Reset();
        
            bool IsPaused() const;
            bool IsStarted() const;
        
            float ElapsedSeconds() const;
        
        private:
            unsigned long startTicks;
            unsigned long pausedTicks;
            bool paused;
            bool started;
    };
}

#endif
