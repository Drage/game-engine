
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <string>

namespace DrageEngine
{
    class Renderable
    {
        public:
            virtual void Render() const = 0;
    };
}

#endif
