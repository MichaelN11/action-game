#pragma once

#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <array>
#include <cassert>

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
	// not every component has a vector. empty until getComponentList called
	std::vector<std::shared_ptr<ComponentType>> componentList;
	// not every component has a sorted multimap, empty unless addcomponent with sort key is called
	std::multimap<int, std::shared_ptr<ComponentType>> componentSorted;

	void removeEntity(int entityId)
	{

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

		for (auto mMapIt = componentSorted.begin(); mMapIt != componentSorted.end(); ++mMapIt)
		{
			if (mMapIt->second->entityId == entityId)
			{
				componentSorted.erase(mMapIt);
				break;
			}
		}
	}
};

class ComponentManager
{
private:
	const static int MAX_COMPONENT_TYPES = 100;

	static int getNextId()
	{
		static int id = 0;
		++id;
		assert(id < MAX_COMPONENT_TYPES);
		return id;
	}

	template<typename ComponentType>
	static int getComponentTypeId()
	{
		static_assert(std::is_base_of<Component, ComponentType>::value, "type parameter of this class must derive from Component");
		static int componentTypeId = getNextId();
		return componentTypeId;
	}
public:
	class EntityComponents
	{
	public:
		template<typename ComponentType>
		ComponentType* getComponent()
		{
			int componentTypeId = getComponentTypeId<ComponentType>();
			Component* cPtr = componentArray[componentTypeId].get();
			ComponentType* tPtr = static_cast<ComponentType*>(cPtr);
			return tPtr;
		}

	private:
		std::array<std::shared_ptr<Component>, MAX_COMPONENT_TYPES> componentArray;

		template<typename ComponentType>
		std::shared_ptr<ComponentType> getComponentShared()
		{
			int componentTypeId = getComponentTypeId<ComponentType>();
			return std::static_pointer_cast<ComponentType, Component>(componentArray[componentTypeId]);
		}

		template<typename ComponentType>
		void removeComponent()
		{
			int componentTypeId = getComponentTypeId<ComponentType>();
			componentArray[componentTypeId] = nullptr;
		}

		template<typename ComponentType>
		void addComponent(std::shared_ptr<ComponentType> component)
		{
			int componentTypeId = getComponentTypeId<ComponentType>();
			componentArray[componentTypeId] = std::static_pointer_cast<Component, ComponentType>(component);
		}

		friend class ComponentManager;
	};

	// Adds shared ptr to component to the entity's component collection, and if the vector for the component type is already filled, adds it to that too
	template<typename ComponentType>
	void addComponent(ComponentType component)
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		std::shared_ptr<ComponentType> componentPtr = std::make_shared<ComponentType>(component);

		if (holder->componentList.size() > 0)
		{
			holder->componentList.push_back(componentPtr);
		}

		addComponentToEntityComponents(component.entityId, componentPtr);
	}

	// adds a component, and also adds it to a (sorted) multimap which can be used to retrieve the components in order of the keys
	template<typename ComponentType>
	void addComponent(ComponentType component, int sortKey)
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		std::shared_ptr<ComponentType> componentPtr = std::make_shared<ComponentType>(component);

		// add to sorted multimap
		holder->componentSorted.emplace(sortKey, componentPtr);

		if (holder->componentList.size() > 0)
		{
			holder->componentList.push_back(componentPtr);
		}

		addComponentToEntityComponents(component.entityId, componentPtr);
	}

	// sprite components automatically sorted by layer
	template<>
	void addComponent(SpriteComponent component)
	{
		addComponent(component, component.layer);
	}

	EntityComponents* getEntityComponents(int entityId)
	{
		auto it = entityIdMap.find(entityId);
		if (it != entityIdMap.end())
		{
			return &(it->second);
		}
		else
		{
			return nullptr;
		}
	}

	template<typename ComponentType>
	ComponentType* getComponent(int entityId)
	{
		EntityComponents* entity = getEntityComponents(entityId);

		return entity->getComponent<ComponentType>();
	}

	// If vector of componenttype is already filled, returns reference, otherwise fills it from map then returns reference
	template<typename ComponentType>
	std::vector<std::shared_ptr<ComponentType>>& getComponentList()
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		if (holder->componentList.size() == 0)
		{
			for (auto it = entityIdMap.begin(); it != entityIdMap.end(); ++it)
			{
				auto sharedPtr = it->second.getComponentShared<ComponentType>();
				if (sharedPtr)
				{
					holder->componentList.push_back(sharedPtr);
				}
			}
		}
		return holder->componentList;
	}

	// gets the sorted map of components. must already be filled
	template<typename ComponentType>
	std::multimap<int, std::shared_ptr<ComponentType>>& getComponentSorted()
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();
		return holder->componentSorted;
	}

	template<typename ComponentType>
	void removeComponent(int entityId)
	{
		ComponentHolder<ComponentType>* holder = getHolder<ComponentType>();

		holder->removeEntity(entityId);

		auto it = entityIdMap.find(entityId);
		if (it != entityIdMap.end())
		{
			it->second.removeComponent<ComponentType>();
		}
	}

	void removeAllComponents(int entityId)
	{
		for (auto it = componentHolderArray.begin(); it != componentHolderArray.end(); ++it)
		{
			it->get()->removeEntity(entityId);
		}

		auto it = entityIdMap.find(entityId);
		if (it != entityIdMap.end())
		{
			entityIdMap.erase(it);
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
	//std::unordered_map<int, std::unique_ptr<IComponentHolder>> componentHolderMap;
	std::array<std::unique_ptr<IComponentHolder>, MAX_COMPONENT_TYPES> componentHolderArray;
	std::unordered_map<int, EntityComponents> entityIdMap;

	template<typename ComponentType>
	ComponentHolder<ComponentType>* getHolder()
	{
		int componentTypeId = ComponentManager::getComponentTypeId<ComponentType>();
		auto& uniquePtr = componentHolderArray.at(componentTypeId);
		if (!uniquePtr)
		{
			uniquePtr = std::make_unique<ComponentHolder<ComponentType>>();
		}
		IComponentHolder* iPtr = uniquePtr.get();
		ComponentHolder<ComponentType>* hPtr = static_cast<ComponentHolder<ComponentType>*>(iPtr);
		return hPtr;
	}


	template<typename ComponentType>
	void addComponentToEntityComponents(int entityId, std::shared_ptr<ComponentType> componentPtr)
	{
		EntityComponents* entity;
		auto it = entityIdMap.find(entityId);
		if (it == entityIdMap.end())
		{
			entityIdMap.emplace(entityId, EntityComponents());
			entity = &entityIdMap.at(entityId);
		}
		else
		{
			entity = &(it->second);
		}
		entity->addComponent<ComponentType>(std::move(componentPtr));
	}


};

