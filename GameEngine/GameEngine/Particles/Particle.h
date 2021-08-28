
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Material.h"
#include "Vector3.h"
#include "Color.h"
#include "MathUtils.h"

namespace DrageEngine
{
    class Particle
    {
        public:
            Particle(
                const Vector3 &position,
                float size,
                float rotation,
                float lifeTime,
                const Vector3 &velocity,
                const Color &color,
                float angularVelocity,
                float gravity);
        
            void AnimateScale(float start, float end, float inPercent, float outPercent);
            void AnimateColor(const Color &start, const Color &end, float inPercent, float outPercent);
            void AnimateAlpha(float start, float end, float inPercent, float outPercent);
        
            void Update();
        
        private:
            template<typename T>
            T Animate(T start, T normal, T end, float inPercent, float outPercent, float percent)
            {
                if (percent < inPercent)
                    return Lerp(start, normal, percent / inPercent);
                else if (percent > outPercent)
                    return Lerp(normal, end, (percent - outPercent) / (1 - outPercent));
                else
                    return normal;
            }
        
            Vector3 position;
            float rotation;
            float scale;
        
            float birthTime;
            float deathTime;
            bool destroyed;
            
            Vector3 velocity;
            float angularVelocity;
            float gravity;
        
            Color color;
            float alpha;
        
            bool animateColor;
            Color startColor;
            Color normalColor;
            Color endColor;
            float colorInPercent;
            float colorOutPercent;
        
            bool animateScale;
            float startScale;
            float normalScale;
            float endScale;
            float scaleInPercent;
            float scaleOutPercent;
        
            bool animateAlpha;
            float startAlpha;
            float normalAlpha;
            float endAlpha;
            float alphaInPercent;
            float alphaOutPercent;
    };
}

#endif
