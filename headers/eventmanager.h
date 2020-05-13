#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "event.h"

/*
	EventManager class
	Keeps static lists of "event listeners" as functions for each event type
	Can register listeners and fire events
*/

class EventManager
{
public:
	template<typename EventType>
	static void registerListener(std::function<void(EventType&)> listener)
	{
		static_assert(std::is_base_of<Event, EventType>::value, "type parameter of this class must derive from Event");
		listenerList<EventType>.push_back(listener);
	}

	template<typename EventType>
	static void fireEvent(EventType& pEvent)
	{
		for (std::function<void(EventType&)> listener : listenerList<EventType>)
		{
			listener(pEvent);
		}
	}

	template<typename EventType>
	static void clearListeners()
	{
		listenerList<EventType>.clear();
	}

private:
	template<typename EventType>
	static std::vector<std::function<void(EventType&)>> listenerList;
};