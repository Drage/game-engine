
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
    m_particleMaterial = material;
    m_particleLifetime = lifetime;
    m_particleSize = size;
    m_particleVelocity = velocity;
    m_particleAngularVelocity = angularVelocity;
    m_particleAcceleration = acceleration;
    m_particleDirection = direction;
    m_particleDirectionInfluence = directionInfluence;
}

void SpawnParticle()
{
    // Vector3 spawnLocation = m_volume.GetSpawnLocation();
    
    
}
