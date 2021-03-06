#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <array>
#include <cassert>

#include "event.h"

/*
	EventManager class
	Keeps lists of "event listeners" as functions for each event type in EventHandlers
	Can register listeners, fire events, and clear listeners
*/

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
		EventHandler<EventType>* handler = getHandler<EventType>();
		handler->insert(listener);
	}

	template<typename EventType>
	void fireEvent(EventType pEvent)
	{
		EventHandler<EventType>* handler = getHandler<EventType>();
		handler->fireEvent(pEvent);
	}

	template<typename EventType>
	void clearListeners()
	{
		EventHandler<EventType>* handler = getHandler<EventType>();
		handler->clear();
	}

private:
	//std::unordered_map<int, std::unique_ptr<IEventHandler>> handlerMap;

	const static int MAX_EVENT_TYPES = 100;
	std::array<std::unique_ptr<IEventHandler>, MAX_EVENT_TYPES> eventHandlerArray;

	static int getNextId()
	{
		static int id = 0;
		++id;
		assert(id < MAX_EVENT_TYPES);
		return id;
	}

	template<typename EventType>
	static int getEventTypeId()
	{
		static_assert(std::is_base_of<Event, EventType>::value, "type parameter of this class must derive from Event");
		static int eventTypeId = EventManager::getNextId();
		return eventTypeId;
	}

	template<typename EventType>
	EventHandler<EventType>* getHandler()
	{
		int eventTypeId = EventManager::getEventTypeId<EventType>();
		auto& uniquePtr = eventHandlerArray.at(eventTypeId);
		if (!uniquePtr)
		{
			uniquePtr = std::make_unique<EventHandler<EventType>>();
		}
		IEventHandler* iPtr = uniquePtr.get();
		EventHandler<EventType>* hPtr = static_cast<EventHandler<EventType>*>(iPtr);
		return hPtr;
	}

};