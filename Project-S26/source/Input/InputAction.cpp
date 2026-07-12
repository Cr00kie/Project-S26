#include "InputAction.h"

void InputAction::addSourceX(InputDevice device, int code, float scale)
{
	if (scale != 0)
	{
		InputSource srcX{
		device,
		code,
		scale
		};

		xBindings.push_back(srcX);
	}
}

void InputAction::addSourceY(InputDevice device, int code, float scale)
{
	if (scale != 0)
	{
		InputSource srcY{
		device,
		code,
		scale
		};

		yBindings.push_back(srcY);
	}
}

InputAction::InputAction(int playerIndex, float deadzone)
	: playerIndex(playerIndex), deadzone(deadzone), valueX(0), valueY(0), heldPrevFrame(false)
{ 
}

// should be called every frame
void InputAction::evaluateBindings(const InputState& state)
{
	heldPrevFrame = isPressed();
	valueX = 0;
	valueY = 0;

	for (InputSource& source : xBindings) valueX += source.evaluateSource(state, playerIndex);
	for (InputSource& source : yBindings) valueY += source.evaluateSource(state, playerIndex);

	float magnitudeSqr = valueX * valueX + valueY * valueY;
	if (magnitudeSqr < deadzone * deadzone)
	{
		valueX = 0;
		valueY = 0;
	}
	else
	{
		float magnitude = std::sqrt(magnitudeSqr);
		if (magnitude > 0.f)
		{
			float normalizedMag = std::clamp((magnitude - deadzone) / (1.f - deadzone), 0.f, 1.f);
			valueX = (valueX / magnitude) * normalizedMag;
			valueY = (valueY / magnitude) * normalizedMag;
		}
	}

	valueX = std::clamp(valueX, -1.f, 1.f);
	valueY = std::clamp(valueY, -1.f, 1.f);
}

InputAction& InputAction::addBinding(SDL_Scancode keyScancode, float scaleX, float scaleY)
{
	addSourceX(Key, keyScancode, scaleX);
	addSourceY(Key, keyScancode, scaleY);

	return *this;
}

InputAction& InputAction::addBinding(MouseButtons mouseButton, float scaleX, float scaleY)
{
	addSourceX(MouseButton, mouseButton, scaleX);
	addSourceY(MouseButton, mouseButton, scaleY);

	return *this;
}

InputAction& InputAction::addBinding(SDL_GamepadAxis gamepadAxis, float scaleX, float scaleY)
{
	addSourceX(GamepadAxis, gamepadAxis, scaleX);
	addSourceY(GamepadAxis, gamepadAxis, scaleY);

	return *this;
}

InputAction& InputAction::addBinding(SDL_GamepadButton gamepadButton, float scaleX, float scaleY)
{
	addSourceX(GamepadButton, gamepadButton, scaleX);
	addSourceY(GamepadButton, gamepadButton, scaleY);

	return *this;
}
