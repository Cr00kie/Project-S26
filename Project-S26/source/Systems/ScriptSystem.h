#pragma once

#include "../Entities/Registry.h"
#include "../Components/Script.h"
#include <utility>

class ScriptSystem
{
private:

	template<typename... Args>
	void call(sol::protected_function func, Args&&... args)
	{
		if (!func.valid()) return;

		sol::protected_function_result res = func(std::forward<Args>(args)...);
		if (!res.valid())
		{
			sol::error err = res;
			ERROR(err.what());
		}
	}

public:

	void update(Registry& reg, float dt)
	{
		for (Entity& e : reg.view<Script>())
		{
			Script& s = reg.get<Script>(e);

			if (!s.started)
			{
				call(s.start, s.self, e);

				s.started = true;
			}

			call(s.update, s.self, e, dt);
		}
	}
};