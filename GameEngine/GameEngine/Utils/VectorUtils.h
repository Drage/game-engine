
#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#include <vector>

namespace DrageEngine
{
    class Vector
    {
        public:
            template<typename T>
            static bool Remove(std::vector<T> &list, const T &item)
            {
                typename std::vector<T>::iterator i;
                for (i = list.begin(); i != list.end(); i++)
                {
                    if ((*i) == item)
                    {
                        list.erase(i);
                        return true;
                    }
                }
                return false;
            }
    };
}

#endif
