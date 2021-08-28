
#ifndef GAME_H
#define GAME_H

#include "Renderer.h"

namespace DrageEngine
{
    class Game
    {
        public:
            virtual void Load() = 0;
            virtual void Start() = 0;
            virtual void Update() = 0;
            virtual void Render(Renderer *renderer) = 0;
    };
}

#endif
