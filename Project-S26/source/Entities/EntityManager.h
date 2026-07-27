#pragma once

#include <vector>
#include <cassert>
#include "Entity.h"

class EntityManager
{
private:
	std::vector<unsigned int> m_generations;
	std::vector<unsigned int> m_freeIds;

public:
	Entity create();

	void destroy(Entity e);

	inline bool isValid(Entity e) const
	{
		return e.id < m_generations.size() && m_generations[e.id] == e.generation;
	}
};

