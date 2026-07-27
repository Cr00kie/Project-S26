#pragma once

#include <sol.hpp>
#include <string>
#include <unordered_map>
#include "../Debug.h"

class LuaManager
{
public:
	LuaManager();

	// In order for the macros LUA_EXPOSE_TYPE and LUA_EXPOSE_MEMBER to work, we need a static instance of the sol::state.
	//		However, if we just declare a plain static sol::state member in LuaManager, we cannot be sure that it will be initialized before any of the macros.
	//		Therefore, we make this simple lua() getter with a static instance in the function. This means that whenever it is needed, it will be created.
	static sol::state& lua() { 
		static sol::state instance;
		
		return instance; 
	}

	sol::table getOrLoadModule(const std::string& path);

	sol::table createInstance(sol::table module);

private:
	void bindGlobalServices();
	void bindComponentGetters();

	std::unordered_map<std::string, sol::table> m_modulesCache;
};

