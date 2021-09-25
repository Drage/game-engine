
#ifndef TIMER_H
#define TIMER_H

namespace DrageEngine
{
    class Timer
    {
        public:
            Timer();
            void Start();
            void Reset();
        
            double ElapsedSeconds() const;
            double ElapsedMilliseconds() const;
        
        private:
            bool started;
            unsigned long startTicks;
    };
}

#endif
