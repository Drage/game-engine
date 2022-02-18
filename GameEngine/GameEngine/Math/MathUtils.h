
#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <initializer_list>

namespace DrageEngine
{
    extern const float PI;
    extern const float EPSILON;
    
    float DegToRad(float degrees);
    float RadToDeg(float radians);
    
    template<typename T>
    T Min(T a, T b)
    {
        return (a < b) ? a : b;
    }

    template <typename T, typename ... Args>
    T Min(T a, T b, Args ... args)
    {
        return Min(Min(a, b), args...);
    }
    
    template<typename T>
    T Max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    template <typename T, typename ... Args>
    T Max(T a, T b, Args ... args)
    {
        return Max(Max(a, b), args...);
    }
    
    template<typename T>
    T Clamp(T value, T min, T max)
    {
        if (value < min)
            return min;
        else if (value > max)
            return max;
        else
            return value;
    }
    
    template<typename T>
    T Wrap(T value, T min, T max)
    {
        while (value < min)
            value += (max - min);
        while (value > max)
            value -= (max - min);
        return value;
    }
    
    int PingPong(int value, int min, int max);
    
    template<typename T>
    T Sign(T value)
    {
        return (T(0) < value) - (value < T(0));
    }
    
    template<typename T>
    T Square(T value)
    {
        return value * value;
    }
    
    template<typename T>
    T Cube(T value)
    {
        return value * value * value;
    }

    float Normalize(float value, float min, float max);

    template<typename T>
    T Lerp(T from, T to, float percent)
    {
        return from + (to - from) * percent;
    }
}

#endif
