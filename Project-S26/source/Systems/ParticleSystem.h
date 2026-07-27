#pragma once

#include "../Entities/Registry.h"
#include "../Components/Transform.h"
#include "../Components/ParticleEmitter.h"

class ParticleSystem
{
public:
	void update(Registry& reg, float dt)
	{
		for (Entity e : reg.view<ParticleEmitter>())
		{
			Transform& transform = reg.get<Transform>(e);
			ParticleEmitter& emitter = reg.get<ParticleEmitter>(e);

			if (emitter.m_isEmitting)
			{
				// Check if we need to emit particles
				emitter.m_elapsed += dt;
				if (emitter.m_elapsed > emitter.m_emissionRate)
				{
					emitter.emit(emitter.m_emissionAmount);
					emitter.m_elapsed = 0;
				}
			}
			else emitter.m_elapsed = 0;

			// Update particles
			for (Particle* particle : emitter.m_activeParticles)
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
			emitter.m_activeParticles.erase(std::remove_if(emitter.m_activeParticles.begin(), emitter.m_activeParticles.end(), [&emitter](Particle* p)
				{
					if (!p->active)
					{
						emitter.m_particlePool.free(p);
						return true;
					}
					return false;
				}), emitter.m_activeParticles.end());
		}
	}

	void render(Registry& reg)
	{
		for (Entity e : reg.view<ParticleEmitter>())
		{
			Transform& transform = reg.get<Transform>(e);
			ParticleEmitter& emitter = reg.get<ParticleEmitter>(e);

			Vec2f emitterPos = transform.worldMatrix.getTranslation();
			float width = (float)emitter.m_particleTexture->getWidth();
			float height = (float)emitter.m_particleTexture->getHeight();

			emitter.m_particleTexture->setTextureColor(emitter.m_particleColor);
			emitter.m_particleTexture->setTextureAlpha(emitter.m_particleColor.a);

			for (Particle* particle : emitter.m_activeParticles)
			{
				float finalX = emitterPos.getX() + particle->position.getX() - (width * transform.scale.getX() * particle->scale) / 2;
				float finalY = emitterPos.getY() + particle->position.getY() - (height * transform.scale.getY() * particle->scale) / 2;
				float totalWidth = (width * transform.scale.getX() * particle->scale);
				float totalHeight = (height * transform.scale.getY() * particle->scale);

				emitter.m_particleTexture->render({ finalX, finalY, totalWidth, totalHeight }, SDL_FLIP_NONE, particle->rotation);
			}
		}
	}
};