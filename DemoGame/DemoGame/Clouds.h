
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
    
        Cloud *m_clouds;
        Billboard m_billboardQuad;
        int m_numClouds;
        float m_orbitRadius;
        float m_minYPos;
        float m_maxYPos;
        float m_minSpeed;
        float m_maxSpeed;
        float m_minSize;
        float m_maxSize;
};

#endif
