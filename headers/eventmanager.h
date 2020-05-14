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

// rework AGAIN to remove static var
// use wrapper class for vector, interface with function template that static casts downward

class IEventHandler
{
public:
	virtual ~IEventHandler() {};
};

template<typename EventType>
class EventHandler : public IEventHandler
{
public:
	void insert(std::function<void(EventType&)> listener)
	{
		listenerList.push_back(listener);
	}
	void fireEvent(EventType& pEvent)
	{
		for (std::function<void(EventType&)> listener : listenerList)
		{
			listener(pEvent);
		}
	}
	void clear()
	{
		listenerList.clear();
	}
private:
	std::vector<std::function<void(EventType&)>> listenerList;
};

class EventManager
{
public:
	template<typename EventType>
	void registerListener(std::function<void(EventType&)> listener)
	{
		int eventTypeId = getEventTypeId<EventType>();
		IEventHandler* iPtr = handlerList.at(eventTypeId).get();
		EventHandler<EventType>* hPtr = static_cast<EventHandler<EventType>*>(iPtr);
		hPtr->insert(listener);
	}

	template<typename EventType>
	void fireEvent(EventType& pEvent)
	{
		int eventTypeId = getEventTypeId<EventType>();
		IEventHandler* iPtr = handlerList.at(eventTypeId).get();
		EventHandler<EventType>* hPtr = static_cast<EventHandler<EventType>*>(iPtr);
		hPtr->fireEvent(pEvent);
	}

	template<typename EventType>
	void clearListeners()
	{
		int eventTypeId = getEventTypeId<EventType>();
		IEventHandler* iPtr = handlerList.at(eventTypeId).get();
		EventHandler<EventType>* hPtr = static_cast<EventHandler<EventType>*>(iPtr);
		hPtr->clear();
	}

private:
	std::vector<std::unique_ptr<IEventHandler>> handlerList;

	int getNextId()
	{
		static int id = 0;
		return id++;
	}

	template<typename EventType>
	int getEventTypeId()
	{
		static_assert(std::is_base_of<Event, EventType>::value, "type parameter of this class must derive from Event");
		static int eventTypeId = getNextId();
		if (eventTypeId >= (int) handlerList.size())
			handlerList.push_back(std::make_unique<EventHandler<EventType>>());
		return eventTypeId;
	}

};