#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <unordered_map>
#include "../Math/Vec2.h"
#include "../Debug.h"
#include "InputContextStack.h"
#include "InputState.h"

/*
THINKING OF THE API

InputContext& ctx = inputManager.actions().createContext("Gameplay");
ctx.createAction("fire", BUTTON)
	.addBinding(KEY, SPACEBAR)
	.addBinding(GAMEPAD, SOUTH)
	.addBinding(MOUSE, CLICK);

InputAction& moveAction = ctx.createAction("move", 1AXIS);
moveAction.addBinding(KEY, A, 1.f)
	.addBinding(KEY, D, -1.f)
	.addBinding(GAMEPAD, LJOY, 1.f);

ctx.createAction("camera", 2AXIS)
	.addBinding(GAMEPAD, RJOY,  1.f, 0.8f);

*/


//Manages the input of the application,
//storing it in a queue and sending it to the SceneManager when requested
class InputManager {
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
