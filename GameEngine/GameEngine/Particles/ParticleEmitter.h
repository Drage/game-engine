
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
            Material *particleMaterial;
            float particleLifetime;
            ParticleAnimParameter particleSize;
            ParticleAnimParameter particleVelocity;
            ParticleAnimParameter particleAngularVelocity;
            ParticleAnimParameter particleAcceleration;
        
            //ParticleEmitterVolume volume;
        
            Vector3 particleDirection;
            float particleDirectionInfluence; // 0 = ignore, 1 = follow direction strictly
    };
}

#endif
