
#ifndef DEMOGAME_H
#define DEMOGAME_H

#include <GameEngine.h>

using namespace DrageEngine;

class DemoGame : public Game
{
    public:
        virtual void Load();
        virtual void Start();
        virtual void Update();
        virtual void Render(Renderer *renderer);
    
    private:
        Scene *scene;
};

#endif
