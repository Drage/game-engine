
#ifndef SINGLETON_H
#define SINGLETON_H

namespace DrageEngine
{
    template<class T>
    class Singleton
    {
        public:
            static T* GetInstance()
            {
                static T instance;
                return &instance;
            }
        
        protected:
            Singleton() {}
            virtual ~Singleton() {}
            Singleton(Singleton const&) {}
            Singleton& operator=(Singleton const&) {}
    };
}

#endif
