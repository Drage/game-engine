
#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Material.h"
#include "ParticleAnimParameter.h"

namespace DrageEngine
{
    class ParticleEmitter
    {
        public:
            ParticleEmitter();
        
            void SetupParticleProperties(
                Material *material,
                float lifetime,
                const ParticleAnimParameter &size,
                const ParticleAnimParameter &velocity,
                const ParticleAnimParameter &angularVelocity,
                const ParticleAnimParameter &acceleration,
                const Vector3 &direction,
                float directionInfluence);
        
            void SpawnParticle();
        
        private:
            Material *m_particleMaterial;
            float m_particleLifetime;
            ParticleAnimParameter m_particleSize;
            ParticleAnimParameter m_particleVelocity;
            ParticleAnimParameter m_particleAngularVelocity;
            ParticleAnimParameter m_particleAcceleration;
        
            //ParticleEmitterVolume m_volume;
        
            Vector3 m_particleDirection;
            float m_particleDirectionInfluence; // 0 = ignore, 1 = follow direction strictly
    };
}

#endif
