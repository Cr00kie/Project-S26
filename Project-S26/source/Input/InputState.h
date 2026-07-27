#pragma once

#include <unordered_map>
#include <array>
#include <optional>
#include <string>
#include <SDL3/SDL.h>

#include "../Lua/LuaBindingTools.h"

// holds information about a button
struct ButtonInput
{
	LUA_EXPOSE_TYPE(ButtonInput, ButtonInput, NO_CONSTRUCTOR, NO_BASE)
	bool held = false;
	bool heldPrevFrame = false;

	inline bool isPressed() const { return held; }
	inline bool justPressed() const { return held && !heldPrevFrame; }
	inline bool justReleased() const { return !held && heldPrevFrame; }
	
	LUA_EXPOSE_MEMBER(ButtonInput, isPressed, is_pressed)
	LUA_EXPOSE_MEMBER(ButtonInput, justPressed, just_pressed)
	LUA_EXPOSE_MEMBER(ButtonInput, justReleased, just_released)
};

// holds information about an axis
struct AxisInput
{
	LUA_EXPOSE_TYPE(AxisInput, AxisInput, NO_CONSTRUCTOR, NO_BASE)

	float value = 0.f;
	LUA_EXPOSE_MEMBER(AxisInput, value, value)
	float valuePrevFrame = 0.f;
	inline float delta() const { return value - valuePrevFrame; }
	LUA_EXPOSE_MEMBER(AxisInput, delta, delta)
};

// holds information about a scroll
struct ScrollInput
{
	LUA_EXPOSE_TYPE(ScrollInput, ScrollInput, NO_CONSTRUCTOR, NO_BASE)

	float deltaX = 0.f;
	LUA_EXPOSE_MEMBER(ScrollInput, deltaX, delta_x)
	float deltaY = 0.f;
	LUA_EXPOSE_MEMBER(ScrollInput, deltaY, delta_y)
};

// holds mouse state
struct Mouse
{
	LUA_EXPOSE_TYPE(Mouse, mouse, NO_CONSTRUCTOR, NO_BASE)

	float x = 0.f, y = 0.f, prevX = 0.f, prevY = 0.f;
	LUA_EXPOSE_MEMBER(Mouse, x, x)
	LUA_EXPOSE_MEMBER(Mouse, y, y)
	LUA_EXPOSE_MEMBER(Mouse, prevX, prev_x)
	LUA_EXPOSE_MEMBER(Mouse, prevY, prev_y)

	ScrollInput scroll;
	LUA_EXPOSE_MEMBER(Mouse, scroll, scroll)

	ButtonInput right, left, middle;
	LUA_EXPOSE_MEMBER(Mouse, right, right)
	LUA_EXPOSE_MEMBER(Mouse, left, left)
	LUA_EXPOSE_MEMBER(Mouse, middle, middle)

	void refresh();
};

// holds keyboard state
struct Keyboard
{
	LUA_EXPOSE_TYPE(Keyboard, Keyboard, NO_CONSTRUCTOR, NO_BASE)

	ButtonInput keys[SDL_SCANCODE_COUNT];

	const ButtonInput& getKey(const std::string& code)
	{
		return keys[SDL_GetScancodeFromName(code.c_str())];
	}
	LUA_EXPOSE_MEMBER(Keyboard, getKey, key)

	void refresh();
};

// holds gamepad state
struct Gamepad
{
	LUA_EXPOSE_TYPE(Gamepad, Gamepad, NO_CONSTRUCTOR, NO_BASE)

	SDL_Gamepad* sdlGamepad;
	ButtonInput buttons[SDL_GAMEPAD_BUTTON_COUNT];
	AxisInput axes[SDL_GAMEPAD_AXIS_COUNT];

	ButtonInput& getButton(SDL_GamepadButton button)
	{
		return buttons[button];
	}
	LUA_EXPOSE_MEMBER(Gamepad, getButton, button)

	AxisInput& getAxis(SDL_GamepadAxis axis)
	{
		return axes[axis];
	}
	LUA_EXPOSE_MEMBER(Gamepad, getAxis, axis)

	void refresh();
};

// holds all input devices state
struct InputState{
	LUA_EXPOSE_TYPE(InputState, InputState, NO_CONSTRUCTOR, NO_BASE)

	Keyboard keyboard;
	LUA_EXPOSE_MEMBER(InputState, keyboard, keyboard)

	Mouse mouse;
	LUA_EXPOSE_MEMBER(InputState, mouse, mouse)

	static constexpr int MAX_PLAYERS = 4;
	std::array<std::optional<SDL_JoystickID>, MAX_PLAYERS> playerSlots;
	
	std::unordered_map<SDL_JoystickID, Gamepad> gamepads;

	Gamepad& getGamepad(int playerIndex)
	{
		return gamepads.at(playerSlots[playerIndex].value());
	}
	LUA_EXPOSE_MEMBER(InputState, getGamepad, gamepad)
};

