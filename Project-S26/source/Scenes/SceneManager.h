#pragma once

#include <map>
#include <string>
#include <vector>
#include "../Input/InputState.h"

class Event;
class UIElement;

class SceneManager
{
private:
	using SceneKey = std::string;
	struct Scene
	{
		SceneKey key;
		UIElement* root;
		bool update = true;
		bool render = true;
		bool captureEvents = false;
		bool active = false;
		int order = 0;
		friend bool operator< (const Scene& s1, const Scene& s2)
		{
			return s1.order < s2.order;
		}
	};
	
	std::map<SceneKey, Scene> m_scenes;
	std::vector<Scene*> m_activeScenes;
	std::vector<Scene*> m_pendingToActivateScenes;

	void cleanActiveScenes();
	void pourPendingScenes();

public:
	SceneManager();
	~SceneManager();

	void processInput(const InputState& state);
	void onEvent(Event& ev);
	void render();
	void update(float dt);

	void addScene(const SceneKey& name, UIElement* root, bool captureEvents = false, bool update = true, bool render = true);

	bool isActive(const SceneKey& name);
	void activateScene(const SceneKey& name, int order);
	void deactivateScene(const SceneKey& name);

	void setSceneUpdate(const SceneKey& name, bool update);
	void setSceneRender(const SceneKey& name, bool render);
	void setSceneCaptureEvents(const SceneKey& name, bool captures);
};

