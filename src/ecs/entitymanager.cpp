#include "ecs/entitymanager.h"
#include "ecs/componentmanager.h"
#include "config.h"

// REMOVE LATER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <iostream>

EntityManager::EntityManager(ComponentManager& compManager) : compManager(compManager)
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
	PositionComponent position(entityId, x, y);
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

void EntityManager::createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data, DrawState drawState, Direction direction)
{
	std::cout << "Entity # " << entityId << " created." << std::endl;

	StateComponent stateComponent(entityId, drawState, 100);

	if (data.spritePath != "")
	{
		compManager.addComponent(SpriteComponent(entityId, data.spritePath, data.tileNum, data.spriteWidth, data.spriteHeight, data.spriteLayer));
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
	if (data.animationMap)
	{
		compManager.addComponent(AnimationComponent(entityId, data.animationTimeToUpdate, data.animationMap.get()));
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

	compManager.addComponent(stateComponent);
}