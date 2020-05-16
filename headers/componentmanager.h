#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "component.h"

class IComponentHolder
{
public:
	virtual ~IComponentHolder() {}
	virtual void removeEntity(int entityId) {}
};

template<typename ComponentType>
class ComponentHolder : public IComponentHolder
{
public:
	// every component has a map. map key is the entity id.
	std::unordered_map<int, std::shared_ptr<ComponentType>> componentMap;
	// not every component has a vector. empty until getComponentList called
	std::vector<std::shared_ptr<ComponentType>> componentList;

	void removeEntity(int entityId)
	{
		auto mapIt = componentMap.find(entityId);

		if (mapIt != componentMap.end())
		{
			componentMap.erase(entityId);
			for (auto vectorIt = componentList.begin(); vectorIt != componentList.end(); ++vectorIt)
			{
				if (vectorIt->get()->entityId == entityId)
				{
					auto lastEl = componentList.end() - 1;
					if (vectorIt != lastEl)
						*vectorIt = std::move(*lastEl);
					componentList.pop_back();
					break;
				}
			}
		}
	}
};

class ComponentManager
{
public:

	// Adds shared ptr to component to the component type's map, and if the vector for the component type is already filled, adds it to that too
	template<typename ComponentType>
	void addComponent(ComponentType component)
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		std::shared_ptr<ComponentType> componentPtr = std::make_shared<ComponentType>(component);

		if (holder->componentList.size() > 0)
		{
			holder->componentList.push_back(componentPtr);
		}

		holder->componentMap.emplace(component.entityId, std::move(componentPtr));
	}

	template<typename ComponentType>
	ComponentType* getComponent(int entityId)
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		auto it = holder->componentMap.find(entityId);

		if (it != holder->componentMap.end())
		{
			return it->second.get();
		}
		else
			return nullptr;
	}

	// If vector of componenttype is already filled, returns reference, otherwise fills it from map then returns reference
	template<typename ComponentType>
	std::vector<std::shared_ptr<ComponentType>>& getComponentList()
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		if (holder->componentList.size() == 0)
		{
			for (auto it = holder->componentMap.begin(); it != holder->componentMap.end(); ++it)
			{
				holder->componentList.push_back(it->second);
			}
		}
		return holder->componentList;
	}

	// Sets component's "exists" field to false and removes it from map
	// component is removed from vector during iteration
	template<typename ComponentType>
	void removeComponent(int entityId)
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		holder->removeEntity(entityId);
	}

	void removeAllComponents(int entityId)
	{
		for (auto it = componentHolderMap.begin(); it != componentHolderMap.end(); ++it)
		{
			it->second.get()->removeEntity(entityId);
		}
	}

	// could be used later for optimization
	//// Checks if component at iterator still exists and is active. If it doesn't exist, it removes it
	//template<typename ComponentType>
	//bool removeBadComponent(std::vector<std::shared_ptr<ComponentType>>& componentList, typename std::vector<std::shared_ptr<ComponentType>>::iterator it)
	//{
	//	if (!(it->get()->exists))
	//	{
	//		auto lastEl = componentList.end() - 1;
	//		if (it != lastEl) 
	//			*it = std::move(*lastEl);
	//		componentList.pop_back();
	//		return true;
	//	}
	//	return false;
	//}

private:
	// map of interface of component holders that gets casted to a holder of component type
	std::unordered_map<int, std::unique_ptr<IComponentHolder>> componentHolderMap;

	static int getNextId()
	{
		static int id = 0;
		return id++;
	}

	template<typename ComponentType>
	static int getComponentTypeId()
	{
		static_assert(std::is_base_of<Component, ComponentType>::value, "type parameter of this class must derive from Component");
		static int eventTypeId = getNextId();
		return eventTypeId;
	}

	template<typename ComponentType>
	ComponentHolder<ComponentType>* getHolder()
	{
		int componentTypeId = ComponentManager::getComponentTypeId<ComponentType>();
		auto it = componentHolderMap.find(componentTypeId);
		if (it == componentHolderMap.end())
			componentHolderMap.emplace(componentTypeId, std::make_unique<ComponentHolder<ComponentType>>());
		IComponentHolder* iPtr = componentHolderMap.at(componentTypeId).get();
		ComponentHolder<ComponentType>* hPtr = static_cast<ComponentHolder<ComponentType>*>(iPtr);
		return hPtr;
	}
};