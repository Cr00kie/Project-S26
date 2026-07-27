#include "LuaManager.h"

#include "../Entities/Entity.h"
#include "../Tools/ServiceLocator.h"
#include "../Debug.h"
#include "../Input/InputManager.h"
#include "../Audio/AudioManager.h"
#include "../World.h"

LuaManager::LuaManager()
{
	lua().open_libraries(sol::lib::base);

	bindGlobalServices();
}

sol::table LuaManager::getOrLoadModule(const std::string& path)
{
	// Try to find module
	auto it = m_modulesCache.find(path);
	if (it != m_modulesCache.end()) return it->second;

	// Try to load module if not found
	// Create environment for script (sandbox script globals)
	sol::environment env(lua(), sol::create, lua().globals());

	// Load script
	sol::protected_function_result result =
		lua().safe_script_file(path, env, sol::script_pass_on_error);
	if (!result.valid())
	{
		sol::error err = result;
		ERROR(err.what());
		return sol::table();
	}

	sol::table module = env;
	m_modulesCache[path] = module;
	return module;
}

sol::table LuaManager::createInstance(sol::table module)
{
	sol::table instance = lua().create_table();
	sol::table meta = lua().create_table();
	meta["__index"] = module;
	instance[sol::metatable_key] = meta;
	return instance;
}

void LuaManager::bindGlobalServices()
{
	lua()["input"] = &ServiceLocator::get<InputManager>();
	lua()["audio"] = &ServiceLocator::get<AudioManager>();
}
