#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "component.h"

class ComponentManager
{
public:
	template<typename ComponentType>
	void addComponent(ComponentType& component)
	{

	}

	template<typename ComponentType>
	ComponentType* getComponent(int entityId)
	{

	}

	template<typename ComponentType>
	std::vector<ComponentType*>& getComponentList()
	{

	}

	template<typename ComponentType>
	void removeComponent(int entityId)
	{

	}

	template<typename ComponentType>
	void removeAllComponents(int entityId)
	{

	}

private:
	// every component has a map. vector index is the component type id. map key is the entity id.
	std::vector<std::unordered_map<int, std::shared_ptr<Component>>> componentMaps;
	// not every component has a vector. map key is component type id.
	std::unordered_map<int, std::vector<std::shared_ptr<Component>>> componentLists;

	int getNextId()
	{
		static int id = 0;
		return id++;
	}

	template<typename ComponentType>
	int getComponentTypeId()
	{
		static_assert(std::is_base_of<Event, EventType>::value, "type parameter of this class must derive from Event");
		static int eventTypeId = getNextId();
		if (eventTypeId >= (int)handlerList.size())
			handlerList.push_back(std::make_unique<EventHandler<EventType>>());
		return eventTypeId;
	}
};