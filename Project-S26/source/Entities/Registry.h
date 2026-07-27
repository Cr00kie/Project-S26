#pragma once

#include <vector>
#include <unordered_map>
#include <utility>
#include "EntityManager.h"
#include "ComponentSparseSet.h"

class Registry
{
private:
	EntityManager m_entityManager;
	std::unordered_map<std::type_index, IComponentSparseSet*> m_pools;

	template <typename T>
	inline ComponentSparseSet<T>& poolFor() { 
		auto key = std::type_index(typeid(T));
		auto it = m_pools.find(key);

		if (it == m_pools.end())
		{
			ComponentSparseSet<T>* newComponentSet = new ComponentSparseSet<T>();
			m_pools.emplace(key, newComponentSet);

			return *newComponentSet;
		}

		return static_cast<ComponentSparseSet<T>&>(*(it->second));
	}

public:
	inline Entity create() { return m_entityManager.create(); }
	inline bool isValid(Entity e) const { return m_entityManager.isValid(e); }

	void destroy(Entity e)
	{
		for (auto& [type, pool] : m_pools) pool->remove(e);
		m_entityManager.destroy(e);
	}

	template <typename T, typename... Args>
	T& emplace(Entity e, Args&&... args) { return poolFor<T>().emplace(e, std::forward<Args>(args)...); }

	template <typename T> T& get(Entity e) { return poolFor<T>().get(e); }
	template <typename T> bool has(Entity e) { return poolFor<T>().has(e); }
	template <typename T> void remove(Entity e) { poolFor<T>().remove(e); }

	template <typename T>
	std::vector<Entity>& view() { return poolFor<T>().entities(); }

	template <typename T>
	std::vector<T>& data() { return poolFor<T>().data(); }
};