#include "SceneManager.h"
#include "../Events/Event.h"
#include "../UI/UIElement.h"
#include "../Math/Mat3.h"
#include "../Debug.h"

#include <algorithm>

void SceneManager::cleanActiveScenes()
{
	// Erase all pending to erase scenes
	m_activeScenes.erase(std::remove_if(m_activeScenes.begin(), m_activeScenes.end(), [this](Scene* s)
		{
			if (!s->active)
			{
				return true;
			}
			return false;
		}), m_activeScenes.end());
}

void SceneManager::pourPendingScenes()
{
	for (Scene* pendingScene : m_pendingToActivateScenes)
	{
		m_activeScenes.push_back(pendingScene);
		pendingScene->active = true;
	}
	m_pendingToActivateScenes.clear();
	std::sort(m_activeScenes.begin(), m_activeScenes.end(), [](const Scene* a, const Scene* b)
		{
			return *a < *b;
		});
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	for (auto& s : m_scenes)
	{
		delete s.second.root;
	}

	m_scenes.clear();
}

void SceneManager::processInput(const InputState& state)
{
	// Mouse movement
	if (state.mouse.x != state.mouse.prevX || state.mouse.y != state.mouse.prevY)
	{
		Event ev{ MouseEvent{ MouseEvent::State::MOVED, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}

	// Left button
	if (state.mouse.left.justPressed())
	{
		Event ev{ MouseEvent{ MouseEvent::State::LEFT_PRESSED, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}
	else if (state.mouse.left.isPressed())
	{
		Event ev{ MouseEvent{ MouseEvent::State::LEFT_PRESSING, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}
	if (state.mouse.left.justReleased())
	{
		Event ev{ MouseEvent{ MouseEvent::State::LEFT_RELEASED, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}

	// Middle button
	if (state.mouse.middle.justPressed())
	{
		Event ev{ MouseEvent{ MouseEvent::State::MIDDLE_PRESSED, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}
	else if (state.mouse.middle.isPressed())
	{
		Event ev{ MouseEvent{ MouseEvent::State::MIDDLE_PRESSING, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}
	if (state.mouse.middle.justReleased())
	{
		Event ev{ MouseEvent{ MouseEvent::State::MIDDLE_RELEASED, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}

	// Right button
	if (state.mouse.right.justPressed())
	{
		Event ev{ MouseEvent{ MouseEvent::State::RIGHT_PRESSED, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}
	else if (state.mouse.right.isPressed())
	{
		Event ev{ MouseEvent{ MouseEvent::State::RIGHT_PRESSING, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}
	if (state.mouse.right.justReleased())
	{
		Event ev{ MouseEvent{ MouseEvent::State::RIGHT_RELEASED, state.mouse.x, state.mouse.y } };
		onEvent(ev);
	}
}

void SceneManager::onEvent(Event& ev)
{
	cleanActiveScenes();

	switch (ev.getType())
	{
	case Event::Type::MOUSE: {
		MouseEvent* mev = ev.getMouse();
		// Find event target
		UIElement* target = nullptr;
		for (Scene* scene : m_activeScenes)
		{
			target = scene->root->findEventTarget(mev->getX(), mev->getY());
			if (target)
			{
				break;
			}
		}
		ev.setEventTarget(target);
	}break;
	case Event::Type::KEY: break;
	default: return;
	}
	
	// Propragate event
	for (Scene* scene : m_activeScenes)
	{
		scene->root->onEvent(ev);
		if (ev.handled() && scene->captureEvents)
		{
			break;
		}
	}
}

void SceneManager::render()
{
	cleanActiveScenes();

	for (auto it = m_activeScenes.rbegin(); it != m_activeScenes.rend(); ++it)
	{
		Scene* scene = (*it);
		if(scene->render) scene->root->render(Mat3f(1.f));
	}
}

void SceneManager::update(float dt)
{
	cleanActiveScenes();
	pourPendingScenes();

	for (Scene* scene : m_activeScenes)
	{
		if (scene->update) scene->root->update(dt);
	}
}

void SceneManager::addScene(const SceneKey& name, UIElement* root, bool captureEvents, bool update, bool render)
{
	if (m_scenes.contains(name))
	{
		DEBUG.error("Scene with name: " + name + ", already exists.\n");
		return;
	}

	m_scenes.insert({ name, {name, root, update, render, captureEvents} });

	LOG("Scene added: " + name + '\n');
}

bool SceneManager::isActive(const SceneKey& name)
{
	return m_scenes.at(name).active;
}

void SceneManager::activateScene(const SceneKey& name, int order)
{
	// Guard against duplicates
	Scene& scene = m_scenes.at(name);
	if (scene.active) return;

	scene.order = order;
	scene.active = true;

	m_pendingToActivateScenes.push_back(&scene);
}

void SceneManager::deactivateScene(const SceneKey& name)
{
	// Linear search should be fine
	m_scenes.at(name).active = false;
}

void SceneManager::setSceneUpdate(const SceneKey& name, bool update)
{
	m_scenes.at(name).update = update;
}

void SceneManager::setSceneRender(const SceneKey& name, bool render)
{
	m_scenes.at(name).render = render;
}

void SceneManager::setSceneCaptureEvents(const SceneKey& name, bool captures)
{
	m_scenes.at(name).captureEvents = captures;
}