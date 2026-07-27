#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <unordered_map>
#include "../Math/Vec2.h"
#include "../Debug.h"
#include "InputContextStack.h"
#include "InputState.h"
#include "../Tools/ServiceLocator.h"

#include "../Lua/LuaBindingTools.h"

// TODO: add support to trigger input actions by pressing multiple buttons at the same time

//Manages the input of the application,
//storing it in a queue and sending it to the SceneManager when requested
class InputManager {
	LUA_EXPOSE_TYPE(InputManager, input, NO_CONSTRUCTOR, NO_BASE)
public:
	//Delete copy constructor and assignment operator to avoid copying the manager
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager&) = delete;

	InputManager();
	~InputManager();

	//send all stored inputs to the SceneManager
	void handleInputs();  

	InputContextStack& actions() { return m_inputContextStack; }
	const InputState& state() { return m_currentInputState; }
	LUA_EXPOSE_MEMBER(InputManager, state, state)

private:
	InputState m_currentInputState;
	InputContextStack m_inputContextStack;

	void refreshState();
	void processEvent(SDL_Event& event);

	void handleGamepadButtonEvent(SDL_Event& event);
	void handleGamepadAxisEvent(SDL_Event& event);
	void handleGamepadDisconnection(SDL_Event& event);
	void handleGamepadConnection(SDL_Event& event);
	void handleMouseWheelEvent(SDL_Event& event);
	void handleMouseButtonEvent(SDL_Event& event);
	void handleMouseMotionEvent(SDL_Event& event);
	void handleKeyboardEvent(SDL_Event& event);
};
