
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
        
            Vector3 m_position;
            float m_rotation;
            float m_scale;
        
            float m_birthTime;
            float m_deathTime;
            bool m_destroyed;
            
            Vector3 m_velocity;
            float m_angularVelocity;
            float m_gravity;
        
            Color m_color;
            float m_alpha;
        
            bool m_animateColor;
            Color m_startColor;
            Color m_normalColor;
            Color m_endColor;
            float m_colorInPercent;
            float m_colorOutPercent;
        
            bool m_animateScale;
            float m_startScale;
            float m_normalScale;
            float m_endScale;
            float m_scaleInPercent;
            float m_scaleOutPercent;
        
            bool m_animateAlpha;
            float m_startAlpha;
            float m_normalAlpha;
            float m_endAlpha;
            float m_alphaInPercent;
            float m_alphaOutPercent;
    };
}

#endif
