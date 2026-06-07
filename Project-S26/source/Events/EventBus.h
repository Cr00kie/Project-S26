#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>
#include <list>
#include <string>
#include <tuple>
#include <utility>

// A class that represents the arguments of an event
template<typename... Targs>
class EventArgs
{
public:
	// Creates simple class to use as event arguments
	// Arguments:
	//		- args: all the data contained in the event arguments
	EventArgs(Targs&& ...args) : data(std::forward<Targs>(args)...)
	{}

	// TODO: Make an interface to access the data easily and make data private
	std::tuple<Targs...> data;
};

// A class that represents a subscription to an event in the EventBus
// Handles unsubscribing from an event in the EventBus
// Can unsubscribe by calling unsubscribe or just when going out of scope
class Subscription
{
private:
	std::function<void()> unsubscribeFunction = nullptr;


public:
	Subscription(){}
	// Creates a Subscription object used to unsubscribe from events
	// SUBSCRIPTIONS OBJECTS ARE RETURNED BY EVENT BUSES OR OTHER CLASSES.
	// Arguments:
	//		- fn: the function which unsubscribes from the event
	Subscription(std::function<void()> fn) : unsubscribeFunction(fn) {}

	//Subscription(Subscription&) = delete;
	//Subscription& operator=(const Subscription&) = delete;

	//unsubscribe from the event by calling the unsubscribe function, if it exists
	void unsubscribe()
	{
		if (unsubscribeFunction)
		{
			unsubscribeFunction();
			unsubscribeFunction = nullptr;
		}
	}
};

// A class that represents an event bus that allows subscribing to and publishing events by id
class EventBus
{
public:
	// structure that represents the functions that handle the event and whether they are active. When an event is set to active = false, that event will be removed when possible.
	struct EventHandler
	{
		std::function<void(void*)> fn;
		bool active;
	};

private:
	// Unordered map that stores, per event, a vector of subscribers that listen to it
	std::unordered_map<std::string, std::list<EventHandler>> m_Subscribers;

public:
	// Subscribe to an event with the given id and listener function,
	// returns a Subscription object that can be used to unsubscribe from the event
	// The listener function will be called with the event arguments when the event is published
	// template parameter Event is the type of the event arguments
	template <typename Event>
	Subscription subscribe(const std::string& id, std::function<void(Event&)> listener) {
		// Add the handler to the subscribers list
		m_Subscribers[id].push_back(
			EventHandler{
					[listener](void* event) { listener(*static_cast<Event*>(event)); },
					true
			}
		);

		// Save the anchor to later unsubscribe from the list
		auto itHandler = --m_Subscribers[id].end();
		return Subscription([this, id, itHandler]()
			{
				itHandler->active = false;
			});
	}

	// Publish an event with the given id and event arguments
	// template parameter Event is the type of the event arguments,
	// it will be used to call the listener functions with the correct type
	template<typename Event>
	void publish(const std::string& id, Event& event)
	{
		// Try to find the event
		auto itSusbcribers = m_Subscribers.find(id);
		if (itSusbcribers == m_Subscribers.end()) return;

		// Erase inactive elements
		std::erase_if(itSusbcribers->second, [](const EventHandler& e) { return e.active == false; });

		// Call each event handler for said event
		for (const auto& listener : itSusbcribers->second)
		{
			if(listener.active) listener.fn(&event);
		}
	}

	// Clear all event listeners from the event bus
	inline void clearAllEventListeners()
	{
		m_Subscribers.clear();
	}

	// Clear all event listeners for the event with the given id
	inline void clearEventListeners(const std::string& id)
	{
		m_Subscribers.erase(id);
	}
};