
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
                m_data = NULL;
                m_width = 0;
                m_height = 0;
            }

            Array2D(int width, int height)
            {
                m_data = NULL;
                Allocate(width, height);
            }

            ~Array2D()
            {
                Deallocate();
            }

            bool Allocate(int width, int height)
            {
                if (m_data)
                    Deallocate();

                m_width = width;
                m_height = height;
    
                try
                {
                    m_data = new T*[m_height];

                    for (int i = 0; i < m_height; i++)
                        m_data[i] = new T[m_width];

                    return true;
                }
                catch (...)
                {
                    return false;
                }
            }

            void Deallocate()
            {
                if (m_data)
                {
                    for (int i = 0; i < m_height; i++)
                        delete[] m_data[i];

                    delete[] m_data;

                    m_data = NULL;
                }
            }

            bool IsAllocated() const
            {
                return m_data != NULL;
            }

            int GetWidth() const
            {
                return m_width;
            }

            int GetHeight() const
            {
                return m_height;
            }

            T& operator() (int x, int y)
            {
                if (x >= m_width || y >= m_height || x < 0 || y < 0)
                {
                    ERROR("Array index out of bounds");
                    return m_dummyValue;
                }

                return m_data[y][x];
            }

            T operator() (int x, int y) const
            {
                if (x >= m_width || y >= m_height || x < 0 || y < 0)
                {
                    ERROR("Array index out of bounds");
                    return m_dummyValue;
                }

                return m_data[y][x];
            }

        private:
            T** m_data;

            int m_width;
            int m_height;

            T m_dummyValue;
    };
}

#endif
