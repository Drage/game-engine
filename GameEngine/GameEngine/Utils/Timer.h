
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
            unsigned long m_startTicks;
            unsigned long m_pausedTicks;
            bool m_paused;
            bool m_started;
    };
}

#endif
