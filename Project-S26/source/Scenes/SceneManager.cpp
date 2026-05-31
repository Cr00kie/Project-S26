#include "SceneManager.h"
#include "../Events/Event.h"
#include "../UI/UIElement.h"
#include "../Math/Mat3.h"

SceneManager::SceneManager() :
	m_currentScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	for (auto& s : m_scenes)
	{
		delete s.second;
	}

	m_scenes.clear();
}

void SceneManager::onEvent(Event& ev)
{
	switch (ev.getType())
	{
	case Event::Type::MOUSE: {
		MouseEvent* mev = ev.getMouse();
		// Find event target
		UIElement* target = m_currentScene->findEventTarget(mev->getX(), mev->getY());
		ev.setEventTarget(target);
	}break;
	case Event::Type::KEY: break;
	default: return;
	}
	
	// Propragate event
	m_currentScene->onEvent(ev);
}

void SceneManager::render()
{
	m_currentScene->render(Mat3f(1.f));
}

void SceneManager::update(float dt)
{
	m_currentScene->update(dt);
}

void SceneManager::addScene(const SceneKey& name, Scene* scene)
{
	m_scenes.insert({ name, scene });

	if (!m_currentScene)
	{
		m_currentScene = scene;
	}
}

void SceneManager::setCurrentScene(const SceneKey& name)
{
	m_currentScene = m_scenes[name];
}
