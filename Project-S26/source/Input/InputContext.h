#pragma once

#include "InputAction.h"
#include "InputState.h"
#include <unordered_map>
#include <string>

class InputContext
{
private:
	std::unordered_map<std::string, InputAction> actions;
	bool consumeInput = true;

public:
	void updateActions(const InputState& state);
	InputAction& createAction(const std::string& name, int playerIndex = 0, float deadzone = 0.f);
	InputAction& getAction(const std::string& name);
	inline bool consumesInput() const { return consumeInput; }
	inline void setConsumesInput(bool consumes) { consumeInput = consumes; }
};

