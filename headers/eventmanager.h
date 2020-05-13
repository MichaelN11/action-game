#pragma once

#include <vector>
#include <memory>

#include "event.h"

/*
	EventListener class
	To capture an event, this should be inherited from and the derived class registered with the manager.
	Derived class should only contain onEvent function!
	Can also hold reference to its parent and pass it in constructor
*/

template<typename EventType>
class EventListener
{
public:
	EventListener()
	{
		static_assert(std::is_base_of<Event, EventType>::value, "type parameter of this class must derive from Event");
	}
	virtual ~EventListener() {}

	virtual void onEvent(EventType& pEvent) = 0;
	
};

/*
	EventManager class
	Keeps static lists of event listeners for each event type
	Can register listeners and fire events
*/

class EventManager
{
public:
	template<typename EventType, typename Listener>
	static void registerListener()
	{
		static_assert(std::is_base_of<EventListener<EventType>, Listener>::value, "type parameter of event listener must derive from appropriate event listener");
		listenerList<EventType>.emplace_back(std::make_unique<Listener>());
	}

	template<typename EventType, typename Listener, typename ParentClass>
	static void registerListener(ParentClass& parent)
	{
		static_assert(std::is_base_of<EventListener<EventType>, Listener>::value, "type parameter of event listener must derive from appropriate event listener");
		listenerList<EventType>.emplace_back(std::make_unique<Listener>(parent));
	}

	template<typename EventType>
	static void fireEvent(EventType& pEvent)
	{
		for (std::unique_ptr<EventListener<EventType>>& listener : listenerList<EventType>)
		{
			listener->onEvent(pEvent);
		}
	}

	template<typename EventType>
	static void clearListeners()
	{
		listenerList<EventType>.clear();
	}

private:
	template<typename EventType>
	static std::vector<std::unique_ptr<EventListener<EventType>>> listenerList;
};