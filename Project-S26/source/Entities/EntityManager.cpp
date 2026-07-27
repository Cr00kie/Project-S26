#include "EntityManager.h"

Entity EntityManager::create()
{
	unsigned int id;
	// Try to recycle free ids
	if (!m_freeIds.empty())
	{
		id = m_freeIds.back();
		m_freeIds.pop_back();
	}
	else
	{
		id = m_generations.size();
		m_generations.push_back(0);
	}

	return Entity{ id, m_generations[id] };
}

void EntityManager::destroy(Entity e)
{
	assert(isValid(e));

	++m_generations[e.id];

	m_freeIds.push_back(e.id);
}
