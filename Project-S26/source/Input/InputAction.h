#pragma once

#include <vector>
#include <string>
#include "../Math/Vec2.h"
#include "InputSource.h"
#include "../Lua/LuaBindingTools.h"

class InputAction
{
	LUA_EXPOSE_TYPE(InputAction, InputAction, NO_CONSTRUCTOR, NO_BASE)
private:
	std::vector<InputSource> xBindings; // Used for buttons and single axis
	std::vector<InputSource> yBindings; // Used for x and y axis
	float valueX;
	float valueY;
	bool heldPrevFrame;
	int playerIndex;
	float deadzone;

	void addSourceX(InputDevice device, int code, float scale);
	void addSourceY(InputDevice device, int code, float scale);

public:
	InputAction(int playerIndex, float deadzone);

	// should be called every frame
	void evaluateBindings(const InputState& state);

	// TODO: make our own enums
	InputAction& addBinding(SDL_Scancode keyScancode, float scaleX = 1, float scaleY = 0); // keyboard
	InputAction& addBinding(MouseButtons mouseButton, float scaleX = 1, float scaleY = 0); // Mouse
	InputAction& addBinding(SDL_GamepadAxis gamepadAxis, float scaleX = 1, float scaleY = 0); // Gamepad
	InputAction& addBinding(SDL_GamepadButton gamepadButton, float scaleX = 1, float scaleY = 0); // Gamepad

	bool isPressed()    const { return valueX != 0.0f || valueY != 0.0f; }
	bool justPressed()  const { return isPressed() && !heldPrevFrame; }
	bool justReleased() const { 
		return !isPressed() && heldPrevFrame; }

	bool readBool() const
	{
		return (bool)valueX;
	}
	float readFloat() const
	{
		return valueX;
	}
	Vec2f readVec2f() const
	{
		return { valueX, valueY };
	}
};

