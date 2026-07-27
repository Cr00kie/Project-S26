#include "ParticleEmitter.h"

void ParticleEmitter::emit(std::size_t amount)
{
	Vec2f spawnLocation;
	Vec2f velocity;
	Vec2f acceleration;
	float rotation;
	float angularVelocity;
	float scale;
	float lifetime;
	for (std::size_t i = 0; i < amount; ++i)
	{
		// Get random properties for particle
		spawnLocation = Vec2f(
			(SDL_randf() * 2 - 1) * m_spawnArea.getX(), 
			(SDL_randf() * 2 - 1) * m_spawnArea.getY()
		);
		velocity = Vec2f(
			m_minVelocity.getX() + SDL_randf() * (m_maxVelocity.getX() - m_minVelocity.getX()), 
			m_minVelocity.getY() + SDL_randf() * (m_maxVelocity.getY() - m_minVelocity.getY())
			);
		acceleration	= Vec2f(
			m_minAcceleration.getX() + SDL_randf() * (m_maxAcceleration.getX() - m_minAcceleration.getX()),
			m_minAcceleration.getY() + SDL_randf() * (m_maxAcceleration.getY() - m_minAcceleration.getY())
		);
		rotation		= m_minRotation + SDL_randf() * (m_maxRotation - m_minRotation);
		angularVelocity = m_minAngularVel + SDL_randf() * (m_maxAngularVel - m_minAngularVel);
		scale			= m_minScale + SDL_randf() * (m_maxScale - m_minScale);
		lifetime		= m_minLifetime + SDL_randf() * (m_maxLifetime - m_minLifetime);

		// Create particle
		Particle* particle = m_particlePool.allocate(spawnLocation, velocity, acceleration, rotation, angularVelocity, scale, lifetime, m_particleColor);
		particle->active = true;
		m_activeParticles.push_back(particle);
	}
}