#pragma once
#include <functional>
#include <list>
#include "EventBus.h"

template<typename EventT>
class EventChannel
{
private:
	std::list<std::function<void(EventT&)>> m_eventHandlers;
public:
	EventChannel() = default;
	EventChannel(const EventChannel&) = delete;
	EventChannel& operator=(const EventChannel&) = delete;
	EventChannel(EventChannel&&) = delete;
	EventChannel& operator=(EventChannel&&) = delete;

	Subscription addListener(std::function<void(EventT&)> callback)
	{
		m_eventHandlers.push_back(std::move(callback));
		auto it = --m_eventHandlers.end();
		return Subscription([this, it]() { m_eventHandlers.erase(it); });
	}
	template<typename T, typename Method>
	Subscription addListener(T* obj, Method method)
	{
		return addListener([obj, method](EventT& ev) { (obj->*method)(ev); });
	}

	void invoke(EventT& event)
	{
		auto it = m_eventHandlers.begin();
		while (it != m_eventHandlers.end())
		{
			// Save next iterator in case they autoremove themselves
			auto next = std::next(it);
			(*it)(event);
			it = next;
		}
	}
};

