#pragma once
#include <string>
#include <sol.hpp>

#include "../Tools/ServiceLocator.h"
#include "../Lua/LuaManager.h"

// TODO: Instead of holding one script, make it hold a list of scripts so that one entity can have more than one script (but still only have this component once)
struct Script
{
	Script(const std::string& path) : scriptPath(path) {
		LuaManager& engine = ServiceLocator::get<LuaManager>();
		sol::table module = engine.getOrLoadModule(path);

		self = engine.createInstance(module);
		start = module["start"];
		update = module["update"];
	}

	std::string scriptPath;
	sol::table self;

	sol::protected_function start;
	sol::protected_function update;

	bool started = false;
};