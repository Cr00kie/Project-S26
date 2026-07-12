#include "InputManager.h"
#include <SDL3/SDL.h>
#include "../Debug.h"
#include "../AppFacadeService.h"
#include "../Tools/ServiceLocator.h"

InputManager::InputManager()
{
	// Initialize joysticks
	SDL_InitSubSystem(SDL_INIT_GAMEPAD);
}

InputManager::~InputManager()
{
	for (auto& item : m_currentInputState.gamepads)
	{
		Gamepad& gamepad = item.second;
		SDL_CloseGamepad(gamepad.sdlGamepad);
	}
}

void InputManager::handleInputs() {
	refreshState();

	// If necessary we could add a limit of inputs processed per frame
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		processEvent(event);
	}

	m_inputContextStack.evaluate(m_currentInputState);
}

void InputManager::refreshState()
{
	// refresh mouse
	m_currentInputState.mouse.refresh();

	// refresh keyboard
	m_currentInputState.keyboard.refresh();

	// refresh gamepads
	for (auto& gp : m_currentInputState.gamepads)
	{
		gp.second.refresh();
	}
}

void InputManager::processEvent(SDL_Event& event)
{
	AppFacadeService& app = ServiceLocator::get<AppFacadeService>();
	SDL_ConvertEventToRenderCoordinates(&(app.getRenderer()), &event);

	switch (event.type)
	{
	case SDL_EVENT_QUIT: {
		app.quit();
	}break;

//			----- KEYBOARD -----
	case SDL_EVENT_KEY_DOWN: 
	case SDL_EVENT_KEY_UP: handleKeyboardEvent(event); break;

//			----- MOUSE -----
	case SDL_EVENT_MOUSE_MOTION: handleMouseMotionEvent(event); break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN: 
	case SDL_EVENT_MOUSE_BUTTON_UP: handleMouseButtonEvent(event); break;
	case SDL_EVENT_MOUSE_WHEEL: handleMouseWheelEvent(event); break;


//		    ----- GAMEPAD -----
	case SDL_EVENT_GAMEPAD_ADDED: handleGamepadConnection(event); break;
	case SDL_EVENT_GAMEPAD_REMOVED: handleGamepadDisconnection(event); break;
	case SDL_EVENT_GAMEPAD_AXIS_MOTION: handleGamepadAxisEvent(event); break;
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN: 
	case SDL_EVENT_GAMEPAD_BUTTON_UP: handleGamepadButtonEvent(event); break;
	}
}

void InputManager::handleGamepadButtonEvent(SDL_Event& event)
{
	Gamepad& gamepad = m_currentInputState.gamepads.at(event.gbutton.which);
	gamepad.buttons[event.gbutton.button].held = event.gbutton.down;
	//DEBUG.log(std::string(SDL_GetGamepadName(gamepad.sdlGamepad)) + ' ' + std::to_string(event.gbutton.button) + ": " + std::to_string(event.gbutton.down) + '\n');
}

void InputManager::handleGamepadAxisEvent(SDL_Event& event)
{
	Gamepad& gamepad = m_currentInputState.gamepads.at(event.gaxis.which);
	gamepad.axes[event.gaxis.axis].value = event.gaxis.value;
	//if (std::abs(gamepad.axes[event.gaxis.axis].delta()) > 0)
	//	DEBUG.log(std::string(SDL_GetGamepadName(gamepad.sdlGamepad)) + ' ' + " axis " + std::to_string(event.gaxis.axis) + ": " + std::to_string(value) + '\n');
}

void InputManager::handleGamepadDisconnection(SDL_Event& event)
{
	//LOG(std::string(SDL_GetGamepadName(m_currentInputState.gamepads.at(event.gdevice.which).sdlGamepad)) + " removed: " + std::to_string(event.gdevice.which) + '\n');
	SDL_CloseGamepad(m_currentInputState.gamepads.at(event.gdevice.which).sdlGamepad);
	m_currentInputState.gamepads.erase(event.gdevice.which);
	int i = 0;
	while (i < m_currentInputState.playerSlots.size() && m_currentInputState.playerSlots[i] != event.gdevice.which) i++;

	if (i < m_currentInputState.playerSlots.size()) m_currentInputState.playerSlots[i].reset();
}

void InputManager::handleGamepadConnection(SDL_Event& event)
{
	SDL_Gamepad* sdlGamepad = SDL_OpenGamepad(event.gdevice.which);
	if (SDL_GamepadConnected(sdlGamepad))
	{
		Gamepad gamepad;
		gamepad.sdlGamepad = sdlGamepad;
		m_currentInputState.gamepads.insert({ event.gdevice.which, gamepad });
		
		int i = 0;
		while (i < m_currentInputState.playerSlots.size() && m_currentInputState.playerSlots[i].has_value()) 
			i++;

		if (i < m_currentInputState.playerSlots.size()) m_currentInputState.playerSlots[i] = event.gdevice.which;

		//LOG(std::string(SDL_GetGamepadName(gamepad.sdlGamepad)) + " added: " + std::to_string(event.gdevice.which) + '\n');
	}
	else
	{
		//ERROR(SDL_GetError());
	}
}

void InputManager::handleMouseWheelEvent(SDL_Event& event)
{
	m_currentInputState.mouse.scroll.deltaX = event.wheel.x;
	m_currentInputState.mouse.scroll.deltaY = event.wheel.y;
	//LOG("Wheel scrolled\n");
}

void InputManager::handleMouseButtonEvent(SDL_Event& event)
{
	switch (event.button.button)
	{
	case SDL_BUTTON_LEFT:
		m_currentInputState.mouse.left.held = event.button.down;
		//LOG("Mouse left: " + std::to_string(event.button.down) + '\n');
		break;
	case SDL_BUTTON_RIGHT:
		m_currentInputState.mouse.right.held = event.button.down;
		//LOG("Mouse right: " + std::to_string(event.button.down) + '\n');
		break;
	case SDL_BUTTON_MIDDLE:
		m_currentInputState.mouse.middle.held = event.button.down;
		//LOG("Mouse middle: " + std::to_string(event.button.down) + '\n');
		break;
	}
}

void InputManager::handleMouseMotionEvent(SDL_Event& event)
{
	m_currentInputState.mouse.x = event.motion.x;
	m_currentInputState.mouse.y = event.motion.y;
}

void InputManager::handleKeyboardEvent(SDL_Event& event)
{
	m_currentInputState.keyboard.keys[event.key.scancode].held = event.key.down;
	//DEBUG.log("Key down: " + std::string(SDL_GetKeyName(event.key.key)) + '\n');
}
