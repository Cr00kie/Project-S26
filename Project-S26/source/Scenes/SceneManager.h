#pragma once

#include <unordered_map>
#include <string>

class Event;
class UIElement;

class SceneManager
{
private:
	using SceneKey = std::string;
	using Scene = UIElement;

	std::unordered_map<SceneKey, Scene*> m_scenes;
	Scene* m_currentScene;

public:
	SceneManager();
	~SceneManager();
	void onEvent(Event& ev);
	void render();
	void update(float dt);
	void addScene(const SceneKey& name, Scene* scene);
	void setCurrentScene(const SceneKey& name);
};

