#pragma once

#include <memory>
#include <cassert>
#include <utility>
#include <vector>

template<typename T>
class ObjectPool
{
public:
	ObjectPool(std::size_t size) :
		m_size(size),
		m_lastUsed(size - 1),
		m_used(size),
		m_alloc()
	{
		m_pool = m_alloc.allocate(m_size);
	}

	ObjectPool(ObjectPool&& other) noexcept :
		m_size(other.m_size), m_lastUsed(other.m_lastUsed),
		m_used(other.m_used), m_alloc(other.m_alloc),
		m_pool(other.m_pool)
	{
		other.m_alloc.deallocate(other.m_pool, other.m_size);
		other.m_size = 0;
	}

	ObjectPool& operator=(ObjectPool&& other) noexcept
	{
		if (this == &other) return *this;

		m_size = other.m_size;
		m_lastUsed = other.m_lastUsed;
		m_used = other.m_used;
		m_alloc = other.m_alloc;
		m_pool = other.m_pool;

		other.m_alloc.deallocate(other.m_pool, other.m_size);
		other.m_size = 0;

		return *this;
	}

	ObjectPool(const ObjectPool&) = delete;
	ObjectPool& operator=(const ObjectPool&) = delete;

	~ObjectPool()
	{
		m_alloc.deallocate(m_pool, m_size);
	}

	template<typename ...Ts>
	T* allocate(Ts &&...args)
	{
		auto i = (m_lastUsed + 1) % m_size;

		while (m_used[i] && i != m_lastUsed)
			i = (i + 1) % m_size;

		if (!m_used[i])
		{
			m_used[i] = true;
			::new (m_pool + i) T(std::forward<Ts>(args)...);
			m_lastUsed = i;
			return m_pool + i;
		}

		else return nullptr;
	}

	void free(T* p)
	{
		auto idx = p - m_pool;
		assert(idx >= 0 && (std::size_t)idx < m_size); // bounds check
		assert(m_used[idx]);
		p->~T();
		m_used[idx] = false;
	}

private:
	std::size_t m_size;
	std::size_t m_lastUsed;
	std::vector<bool> m_used;
	T* m_pool;

	std::allocator<T> m_alloc;
};

