#include "ecs/entitymanager.h"
#include "ecs/componentmanager.h"
#include "config.h"

EntityManager::EntityManager(ComponentManager& compManager) : compManager(compManager)
{}

void EntityManager::createEntity(const EntityData& data)
{
	int entityId = getNextEntityId();
	createEntityFromData(compManager, entityId, data);
}

void EntityManager::createEntity(float x, float y, const EntityData& data)
{
	int entityId = getNextEntityId();
	PositionComponent position(entityId, x * config::SPRITE_SCALE, y * config::SPRITE_SCALE);
	compManager.addComponent(position);
	createEntityFromData(compManager, entityId, data);
}

void EntityManager::destroyEntity(int entityId)
{
	compManager.removeAllComponents(entityId);
	unusedEntityIds.push_back(entityId);

	//	std::cout << "Entity # " << entityId << " destroyed." << std::endl;
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
}

void EntityManager::createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data)
{
	//	std::cout << "Entity # " << entityId << " created." << std::endl;

	compManager.addComponent(StateComponent(entityId, DrawState::standDown, 100));

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
	}
	if (data.animationMap)
	{
		compManager.addComponent(AnimationComponent(entityId, data.animationTimeToUpdate, data.animationMap));
	}
	if (data.boundingBox.getW() > 0)
	{
		Rectangle<float> bBox = data.boundingBox;
		bBox.scalePositionAndSize(config::SPRITE_SCALE);
		compManager.addComponent(CollisionComponent(entityId, bBox, data.solid, data.interactable));
	}
	if (data.health > 0)
	{
		compManager.addComponent(HealthComponent(entityId, data.health));
	}
	if (data.damage > 0)
	{
		compManager.addComponent(DamageComponent(entityId, data.damage, data.damageGroups));
	}
	if (data.group != Group::none)
	{
		compManager.addComponent(GroupComponent(entityId, data.group, data.hostileGroups));
	}
}