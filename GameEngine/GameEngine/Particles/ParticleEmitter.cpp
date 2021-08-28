
#include "ParticleEmitter.h"

using namespace DrageEngine;

ParticleEmitter::ParticleEmitter()
{
    
}

void ParticleEmitter::SetupParticleProperties(
     Material *material,
     float lifetime,
     const ParticleAnimParameter &size,
     const ParticleAnimParameter &velocity,
     const ParticleAnimParameter &angularVelocity,
     const ParticleAnimParameter &acceleration,
     const Vector3 &direction,
     float directionInfluence)
{
    particleMaterial = material;
    particleLifetime = lifetime;
    particleSize = size;
    particleVelocity = velocity;
    particleAngularVelocity = angularVelocity;
    particleAcceleration = acceleration;
    particleDirection = direction;
    particleDirectionInfluence = directionInfluence;
}

void SpawnParticle()
{
    // Vector3 spawnLocation = volume.GetSpawnLocation();
    
    
}
