#pragma once

#include "InputState.h"

enum InputDevice { Key, MouseButton, GamepadButton, GamepadAxis };

enum MouseButtons {Left, Middle, Right};

struct InputSource
{
	InputDevice device;
	int code;
	float scale = 1.0f;

	float evaluateSource(const InputState& state, int playerIndex = 0);
};

