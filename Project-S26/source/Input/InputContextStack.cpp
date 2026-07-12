#include "InputContextStack.h"

void InputContextStack::evaluate(const InputState& state)
{
	for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
	{
		InputContext& ctx = m_contexts.at(*it);
		ctx.updateActions(state);
		if (ctx.consumesInput()) break; // topmost blocking context wins — Gameplay never sees input while a modal menu context is pushed
	}
}

InputContext& InputContextStack::createContext(const std::string& name)
{
	auto res = m_contexts.insert({ name, InputContext() });
	// TODO: Check if context was added properly

	return res.first->second;
}

InputContext& InputContextStack::getContext(const std::string& contextName)
{
	return m_contexts.at(contextName);
}