#pragma once

#include <vector>
#include <cassert>
#include <utility>
#include "Entity.h"

class IComponentSparseSet
{
public:
	virtual ~IComponentSparseSet() = default;
	virtual void remove(Entity e) = 0;
};

template <typename T>
class ComponentSparseSet : public IComponentSparseSet
{
private:
	std::vector<unsigned int> m_sparse;
	std::vector<T> m_dense;
	std::vector<Entity> m_denseToEntity;

public:
	template <typename ... Args>
	T& emplace(Entity e, Args&&... args)
	{
		// Check if the sparse vector is big enough
		if (e.id >= m_sparse.size())
		{
			m_sparse.resize(e.id + 1, INVALID_ID);
		}

		// Check if this entity already has this component
		assert(m_sparse[e.id] == INVALID_ID);

		// Create and return component
		m_sparse[e.id] = m_dense.size();
		m_denseToEntity.push_back(e);
		m_dense.emplace_back(std::forward<Args>(args)...);
		return m_dense.back();
	}

	void remove(Entity e) override
	{
		if (!has(e)) return;

		// Swap last component with the one to be deleted
		unsigned int removedDenseIdx = m_sparse[e.id];
		unsigned int denseLast = m_dense.size() - 1;
		Entity lastEntity = m_denseToEntity[denseLast];
		
		// swap removed with last to pop back
		m_dense[removedDenseIdx] = std::move(m_dense[denseLast]);
		m_denseToEntity[removedDenseIdx] = lastEntity;
		m_sparse[lastEntity.id] = removedDenseIdx;

		// remove and reset values
		m_dense.pop_back();
		m_denseToEntity.pop_back();
		m_sparse[e.id] = INVALID_ID;
	}

	T& get(Entity e)
	{
		assert(has(e));

		return m_dense[m_sparse[e.id]];
	}

	bool has(Entity e)
	{
		return e.id < m_sparse.size() && m_sparse[e.id] != INVALID_ID;
	}
	std::vector<T>& data() { return m_dense; }
	std::vector<Entity>& entities() { return m_denseToEntity; }
};

