#pragma once

#include <unordered_map>
#include <array>
#include <optional>
#include <SDL3/SDL.h>

// holds information about a button
struct ButtonInput
{
	bool held = false;
	bool heldPrevFrame = false;

	inline bool isPressed() const { return held; }
	inline bool justPressed() const { return held && !heldPrevFrame; }
	inline bool justReleased() const { return !held && heldPrevFrame; }
};

// holds information about an axis
struct AxisInput
{
	float value = 0.f;
	float valuePrevFrame = 0.f;
	inline float delta() const { return value - valuePrevFrame; }
};

// holds information about a scroll
struct ScrollInput
{
	float deltaX = 0.f;
	float deltaY = 0.f;
};

// holds mouse state
struct Mouse
{
	float x = 0.f, y = 0.f, prevX = 0.f, prevY = 0.f;
	ScrollInput scroll;
	ButtonInput right, left, middle;

	void refresh();
};

// holds keyboard state
struct Keyboard
{
	ButtonInput keys[SDL_SCANCODE_COUNT];

	void refresh();
};

// holds gamepad state
struct Gamepad
{
	SDL_Gamepad* sdlGamepad;
	ButtonInput buttons[SDL_GAMEPAD_BUTTON_COUNT];
	AxisInput axes[SDL_GAMEPAD_AXIS_COUNT];

	void refresh();
};

// holds all input devices state
struct InputState
{

	Keyboard keyboard;
	Mouse mouse;

	static constexpr int MAX_PLAYERS = 4;
	std::array<std::optional<SDL_JoystickID>, MAX_PLAYERS> playerSlots;
	
	std::unordered_map<SDL_JoystickID, Gamepad> gamepads;
};

