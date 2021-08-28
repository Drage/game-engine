
#ifndef ARRAY2D_H
#define ARRAY2D_H

#include "Debug.h"

namespace DrageEngine
{
    template<class T>
    class Array2D
    {
        public:
            Array2D()
            {
                data = NULL;
                width = 0;
                height = 0;
            }

            Array2D(int width, int height)
            {
                data = NULL;
                Allocate(width, height);
            }

            ~Array2D()
            {
                Deallocate();
            }

            bool Allocate(int width, int height)
            {
                if (data)
                    Deallocate();

                this->width = width;
                this->height = height;
    
                try
                {
                    data = new T*[height];

                    for (int i = 0; i < height; i++)
                        data[i] = new T[width];

                    return true;
                }
                catch (...)
                {
                    return false;
                }
            }

            void Deallocate()
            {
                if (data)
                {
                    for (int i = 0; i < height; i++)
                        delete[] data[i];

                    delete[] data;

                    data = NULL;
                }
            }

            bool IsAllocated() const
            {
                return data != NULL;
            }

            int GetWidth() const
            {
                return width;
            }

            int GetHeight() const
            {
                return height;
            }

            T& operator() (int x, int y)
            {
                if (x >= width || y >= height || x < 0 || y < 0)
                {
                    ERROR("Array index out of bounds");
                    return dummyValue;
                }

                return data[y][x];
            }

            T operator() (int x, int y) const
            {
                if (x >= width || y >= height || x < 0 || y < 0)
                {
                    ERROR("Array index out of bounds");
                    return dummyValue;
                }

                return data[y][x];
            }

        private:
            T** data;

            int width;
            int height;

            T dummyValue;
    };
}

#endif
