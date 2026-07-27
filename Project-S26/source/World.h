#pragma once

#include "Entities/Registry.h"
#include "Input/InputState.h"

#include "Systems/TransformSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/SpriteRenderSystem.h"
#include "Systems/SpriteAnimationSystem.h"
#include "Systems/ParticleSystem.h"
#include "Systems/TextRenderSystem.h"
#include "Systems/TextLayoutSystem.h"
#include "Systems/ScriptSystem.h"

class World
{
private:
	Registry* m_registry;

	// Systems
	TransformSystem m_transformSystem;
	SpriteRenderSystem m_spriteRenderSystem;
	InputSystem m_inputSystem;
	ParticleSystem m_particleSystem;
	TextRenderSystem m_textRenderSystem;
	TextLayoutSystem m_textLayoutSystem;
	SpriteAnimationSystem m_spriteAnimationSystem;
	ScriptSystem m_scriptSystem;
	// ...
public:
	World(Registry* reg) : m_registry(reg) {}

	Registry& registry() { return *m_registry; }

	void processInput(const InputState& input)
	{
		// Update input system
		m_inputSystem.update(*m_registry, input);
	}

	void update(float dt)
	{
		// Update systems in order
		// ====================================
		// 1.- Update all transforms
		m_transformSystem.update(*m_registry);

		// 2.- Update particle system
		m_particleSystem.update(*m_registry, dt);

		// 3.- Update texts layout
		m_textLayoutSystem.update(*m_registry);

		// 4.- Update sprite animations
		m_spriteAnimationSystem.update(*m_registry, dt);

		// 5.- Update scripts
		m_scriptSystem.update(*m_registry, dt);
	}

	void render()
	{
		// 1.- Render Sprites
		m_spriteRenderSystem.render(*m_registry);

		// 2.- Render Particles
		m_particleSystem.render(*m_registry);

		// 3.- Render Text
		m_textRenderSystem.render(*m_registry);
	}
};

