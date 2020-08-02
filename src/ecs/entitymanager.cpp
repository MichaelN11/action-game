#include "ecs/entitymanager.h"
#include "ecs/componentmanager.h"
#include "config.h"
#include "eventmanager.h"

// REMOVE LATER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <iostream>

EntityManager::EntityManager(ComponentManager& compManager, EventManager& eventManager) : compManager(compManager), eventManager(eventManager), abilityManager(compManager, eventManager, *this)
{
	std::cout << "only see this once!" << std::endl;
}

int EntityManager::createEntity(const EntityData& data)
{
	int entityId = getNextEntityId();
	createEntityFromData(compManager, entityId, data, DrawState::standDown, Direction::none);

	return entityId;
}

int EntityManager::createEntity(float x, float y, const EntityData& data)
{
	return createEntity(x, y, data, DrawState::standDown, Direction::none);
}


int EntityManager::createEntity(float x, float y, const EntityData& data, DrawState drawState, Direction direction)
{
	int entityId = getNextEntityId();
	PositionComponent position(entityId, x, y, data.width, data.height);
	compManager.addComponent(position);
	createEntityFromData(compManager, entityId, data, drawState, direction);

	std::cout << "at position: X: " << x << "   Y: " << y << std::endl;

	return entityId;
}

void EntityManager::destroyEntity(int entityId)
{
	compManager.removeAllComponents(entityId);
	unusedEntityIds.push_back(entityId);

	std::cout << "Entity # " << entityId << " destroyed." << std::endl;
}

int EntityManager::getNextEntityId()
{
	if (unusedEntityIds.size() > 0)
	{
		int unusedId = unusedEntityIds.at(unusedEntityIds.size() - 1);
		unusedEntityIds.pop_back();
		return unusedId;
	}
	else
	{
		return nextEntityId++;
	}

	return nextEntityId++;
}

// always creates entities facing down
void EntityManager::createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data, DrawState drawState, Direction direction)
{
	std::cout << "Entity # " << entityId << " created." << std::endl;
	
	StateComponent stateComponent(entityId, drawState, Direction::down, 100);

	if (data.animSet.filePath != "")
	{
		compManager.addComponent(SpriteComponent(entityId, data.animSet.filePath, data.animSet.initialTileNum, data.animSet.spriteWidth, data.animSet.spriteHeight, data.animSet.spriteLayer));
	}
	if (data.moveSpeed >= 0)
	{
		compManager.addComponent(MovementComponent(entityId, 0.f, 0.f, data.moveSpeed));
	}
	if (data.player)
	{
		compManager.addComponent(PlayerComponent(entityId));
		stateComponent.timeToDie = 5000;
		stateComponent.invTimeFactor = 4.0f;
	}
	if (data.animSet.animationMap.size() > 0)
	{
		compManager.addComponent(AnimationComponent(entityId, data.animSet.timeToUpdate, &data.animSet.animationMap));
	}
	if (data.boundingBox.getW() > 0)
	{
		Rectangle<float> bBox = data.boundingBox;
		compManager.addComponent(CollisionComponent(entityId, bBox, data.solid, data.interactable));
	}
	if (data.health > 0)
	{
		compManager.addComponent(HealthComponent(entityId, data.health));
	}
	if (data.damage > 0)
	{
		compManager.addComponent(DamageComponent(entityId, data.damage, data.damageGroups, direction));
	}
	if (data.group != Group::none)
	{
		compManager.addComponent(GroupComponent(entityId, data.group, data.hostileGroups));
	}
	if (data.lifetime > 0)
	{
		stateComponent.lifetime = data.lifetime;
	}
	if (data.abilityList.size() > 0)
	{
		AbilityComponent abilities(entityId);
		int i = 1;
		for (std::string abilityName : data.abilityList)
		{
			auto it = abilityManager.abilityMap.find(abilityName);
			if (it != abilityManager.abilityMap.end())
			{
				abilities.abilityMap.insert({ i++, &(it->second) });
			}
		}
		compManager.addComponent(abilities);
	}

	compManager.addComponent(stateComponent);
}