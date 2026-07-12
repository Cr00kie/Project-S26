#include "InputSource.h"

float InputSource::evaluateSource(const InputState& state, int playerIndex)
{
	float result = 0.0f;

	switch (device)
	{
	case InputDevice::Key:
		result = state.keyboard.keys[code].held ? 1.0f : 0.0f;
		break;
	case InputDevice::MouseButton:
		switch (code)
		{
		case Left:   result = state.mouse.left.held ? 1.0f : 0.0f; break;
		case Right:  result = state.mouse.right.held ? 1.0f : 0.0f; break;
		case Middle: result = state.mouse.middle.held ? 1.0f : 0.0f; break;
		}
		break;
	case InputDevice::GamepadButton: {
		if (state.playerSlots.size() <= playerIndex || !state.playerSlots[playerIndex].has_value()) break;
		SDL_JoystickID gamepadID = state.playerSlots[playerIndex].value();
		result = state.gamepads.at(gamepadID).buttons[code].held ? 1.0f : 0.0f;
		break;
	}
	case InputDevice::GamepadAxis: {
		if (state.playerSlots.size() <= playerIndex || !state.playerSlots[playerIndex].has_value()) break;
		SDL_JoystickID gamepadID = state.playerSlots[playerIndex].value();
		result = state.gamepads.at(gamepadID).axes[code].value / SDL_JOYSTICK_AXIS_MAX;
		break;
	}
	}

	return result * scale;
}
