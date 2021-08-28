
#ifndef CLOUDS_H
#define CLOUDS_H

#include <GameEngine.h>

using namespace DrageEngine;

class Clouds : public Component
{
    public:
        void Init(const ParamList &params);
        void Update();
        void Render(Renderer *renderer, const Transform *transform = NULL) const;
        ~Clouds();
    
    private:
        static const int NUM_CLOUD_TEXTURES = 8;
    
        struct Cloud
        {
            float speed;
            float angle;
            float size;
            Vector3 position;
            Material *material;
        };
    
        Cloud *clouds;
        Billboard billboardQuad;
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
