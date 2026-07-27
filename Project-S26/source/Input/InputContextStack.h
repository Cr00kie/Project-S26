#pragma once
#include <vector>
#include <string>
#include "../Debug.h"
#include "InputState.h"
#include "InputContext.h"

#include "../Lua/LuaBindingTools.h"


class InputContextStack
{

	std::unordered_map<std::string, InputContext> m_contexts;
	std::vector<std::string> m_stack;
public:
	// DONT LIKE THIS API
	void pushActiveContext(const std::string& name) { m_stack.push_back(name); }
	void removeActiveContext(const std::string& name) { std::erase(m_stack, name); }

	void evaluate(const InputState& state);

	InputContext& createContext(const std::string& name);

	InputContext& getContext(const std::string& contextName);
};

