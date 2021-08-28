
#ifndef ARRAYUTILS_H
#define ARRAYUTILS_H

namespace DrageEngine
{
    class Array
    {
        public:
            template<typename T, int size>
            static int Length(T(&)[size]) { return size; }
    };
}

#endif
