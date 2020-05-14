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

template<typename EventType>
class EventManager
{
public:
	static void registerListener(std::function<void(EventType&)> listener)
	{
		static_assert(std::is_base_of<Event, EventType>::value, "type parameter of this class must derive from Event");
		listenerList.push_back(listener);
	}

	static void fireEvent(EventType& pEvent)
	{
		for (std::function<void(EventType&)> listener : listenerList)
		{
			listener(pEvent);
		}
	}

	static void clearListeners()
	{
		listenerList.clear();
	}
private:
	static std::vector<std::function<void(EventType&)>> listenerList;
};

// necessary for linker
template<typename EventType> std::vector<std::function<void(EventType&)>> EventManager<EventType>::listenerList;