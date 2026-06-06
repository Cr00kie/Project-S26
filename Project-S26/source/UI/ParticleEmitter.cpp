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
			getX() + (SDL_randf() * 2 - 1) * m_spawnArea.getX(), 
			getY() + (SDL_randf() * 2 - 1) * m_spawnArea.getY()
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

void ParticleEmitter::update(float dt)
{
	if (m_isEmitting)
	{
		// Check if we need to emit particles
		m_elapsed += dt;
		if (m_elapsed > m_emissionRate)
		{
			emit(m_emissionAmount);
			m_elapsed = 0;
		}
	}
	else m_elapsed = 0;

	// Update particles
	for (Particle* particle : m_activeParticles)
	{
		particle->lifetime += dt;

		// If particle lifetime is exceeded, kill it
		if (particle->lifetime > particle->maxLifetime)
		{
			particle->active = false;
			continue;
		}

		// Update particle position
		particle->velocity += particle->acceleration * dt;
		particle->position += particle->velocity * dt;

		// Update particle rotation
		particle->rotation += particle->angularVelocity * dt;
	}

	// Remove dead particles
	m_activeParticles.erase(std::remove_if(m_activeParticles.begin(), m_activeParticles.end(), [this](Particle* p)
		{
			if (!p->active)
			{
				m_particlePool.free(p);
				return true;
			}
			return false;
		}), m_activeParticles.end());

	UIElement::update(dt);
}

void ParticleEmitter::render(const Mat3f& parentTransform)
{
	float width = (float)m_particleTexture->getWidth();
	float height = (float)m_particleTexture->getHeight();

	for (Particle* particle : m_activeParticles)
	{
		float finalX = particle->position.getX() - (width * m_scale.getX()) / 2;
		float finalY = particle->position.getY() - (height * m_scale.getY()) / 2;
		float totalWidth = (width * m_scale.getX());
		float totalHeight = (height * m_scale.getY());

		m_particleTexture->render({finalX, finalY, totalWidth, totalHeight}, SDL_FLIP_NONE, particle->rotation);
	}

	UIElement::render(parentTransform);
}
