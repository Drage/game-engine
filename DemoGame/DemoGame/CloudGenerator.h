
#ifndef CLOUDGENERATOR_H
#define CLOUDGENERATOR_H

#include <GameEngine.h>
#include <vector>

using namespace DrageEngine;

class CloudGenerator : public Component
{
    public:
        void Init(const ParamList &params);
    
    private:
        static const int NUM_CLOUD_TEXTURES = 8;
        int numClouds;
        float orbitRadius;
        float minYPos;
        float maxYPos;
        float minSpeed;
        float maxSpeed;
        float minSize;
        float maxSize;
};

#endif
