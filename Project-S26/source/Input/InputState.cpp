#include "InputState.h"

void Gamepad::refresh()
{
	// Refresh gamepad buttons
	for (std::size_t i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; i++)
	{
		buttons[i].heldPrevFrame = buttons[i].held;
	}
	// Refresh gamepad axis
	for (std::size_t i = 0; i < SDL_GAMEPAD_AXIS_COUNT; i++)
	{
		axes[i].valuePrevFrame = axes[i].value;
	}
}

void Keyboard::refresh()
{
	// refresh keyboard state
	for (std::size_t i = 0; i < SDL_SCANCODE_COUNT; i++)
	{
		keys[i].heldPrevFrame = keys[i].held;
	}
}

void Mouse::refresh()
{
	// refresh mouse state
	prevX = x;
	prevY = y;
	left.heldPrevFrame = left.held;
	right.heldPrevFrame = right.held;
	middle.heldPrevFrame = middle.held;
}
