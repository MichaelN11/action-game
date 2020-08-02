#pragma once

#include "ecs/componentmanager.h"
#include "ecs/entitydata.h"

class EventManager;
class EntityManager;

namespace effect
{

	class AbilityEffect
	{
	public:
		virtual ~AbilityEffect() {};
		virtual void trigger(ComponentManager& compManager, EntityManager& entityManager, EventManager& eventManager, ComponentManager::EntityComponents* source, float x, float y) = 0;
	};

	class SpawnAdjacentEntity : public AbilityEffect
	{
	public:
		SpawnAdjacentEntity(AnimationSet animSet, Rectangle<float> boundingBox, bool solid, bool interactable, int health, int damage, int lifetime);
		void trigger(ComponentManager& compManager, EntityManager& entityManager, EventManager& eventManager, ComponentManager::EntityComponents* source, float x, float y);
	private:
		EntityData entitySpawned;
	};
}

