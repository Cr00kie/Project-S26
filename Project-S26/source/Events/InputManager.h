#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <queue>
#include "../Events/Event.h"
#include "../AppFacadeService.h"

//Manages the input of the application,
//storing it in a queue and sending it to the SceneManager when requested
class InputManager {
private:
	//Watcher function, called by SDL when an event is generated,
	//it stores the event in the queue
	static bool eventWatcher(void* userdata, SDL_Event* event);

	//queue:
	//Queue of events, stored until they are sent to the SceneManager
	std::queue<Event> m_queue = std::queue<Event>();
	// bools:
	//Whether the left mouse button is currently pressed,
	//used to send mouse pressed events allways while the button is pressed
	//not only when it is first pressed
	bool m_bLeftMousePressed = false;
	bool m_bRightMousePressed = false;
	bool m_bMiddleMousePressed = false;
	// floats:
	//Last mouse position,
	//used to send mouse motion events with the correct position
	float m_fLastMouseX = 0.0f;
	float m_fLastMouseY = 0.0f;
public:
	//Delete copy constructor and assignment operator to avoid copying the manager
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager&) = delete;

	//Constructor
	//Adds the watcher function to the SDL event system
	InputManager();
	//Destructor
	//Removes the watcher function from the SDL event system
	~InputManager();

	//send all stored inputs to the SceneManager
	void handleInputs();
};