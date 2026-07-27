#pragma once

#include "../LuaBindingTools.h"
#include "../../Entities/Registry.h"

template<typename T>
class BaseComponentHandle
{
public:
	BaseComponentHandle(Registry& reg, Entity e) : m_reg(&reg), m_ent(e){}

	bool isValid() const { return m_reg->isValid(m_ent) && m_reg->has<T>(m_ent); }

protected:

	T& get() const
	{
		if (!isValid())
		{
			ERROR("Entity no longer has this handle\n");
		}
		return m_reg->get<T>(m_ent);
	}

	Registry* m_reg;
	Entity m_ent;
};