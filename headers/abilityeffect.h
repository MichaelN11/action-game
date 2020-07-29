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
		SpawnAdjacentEntity(const EntityData& entity);
		void trigger(ComponentManager& compManager, EntityManager& entityManager, EventManager& eventManager, ComponentManager::EntityComponents* source, float x, float y);
	private:
		EntityData entitySpawned;
	};

	// booleans = diagonal, horizontal, vertical flips
	std::unordered_map<DrawState, Animation> createSwordAnims();

	static const EntityData SWORD =
	{
		// sprite file name path
		"content/tilesheets/attack.png",
		// tile number on spritesheet
		2,
		// sprite width
		16,
		// sprite height
		16,
		// sprite layer
		1,
		// move speed
		0.f,
		// is player
		false,
		// animation map
		 createSwordAnims(),
		 // animation time to update
		 150,
		 // bounding box
		 Rectangle<float>(0.f, 0.f, 16.f, 16.f),
		 // solid
		 false,
		 // interactable
		 true,
		 // health
		 -1,
		 // damage
		 10,
		 // damage group
		 { Group::enemy },
		 // group
		 Group::player,
		 // hostile groups
		 { Group::enemy },
		 // lifetime
		 350
	};

}

