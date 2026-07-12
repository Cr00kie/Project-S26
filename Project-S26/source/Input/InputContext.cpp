#include "InputContext.h"

InputAction& InputContext::createAction(const std::string& name, int playerIndex, float deadzone)
{
	// Add action
	auto action = actions.insert({ name, InputAction(playerIndex, deadzone) });
	return action.first->second;
}

InputAction& InputContext::getAction(const std::string& name)
{
	return actions.at(name);
}


void InputContext::updateActions(const InputState& state)
{
	for (auto& [actionName, action] : actions)
	{
		action.evaluateBindings(state);
	}
}