#include "abilityeffect.h"
#include "ecs/entitymanager.h"
#include "eventmanager.h"

effect::SpawnAdjacentEntity::SpawnAdjacentEntity(AnimationSet animSet, Rectangle<float> boundingBox, bool solid, bool interactable, int health, int damage, int lifetime)
{
	entitySpawned.animSet = animSet;
	entitySpawned.boundingBox = boundingBox;
	entitySpawned.solid = solid;
	entitySpawned.interactable = interactable;
	entitySpawned.health = health;
	entitySpawned.damage = damage;
	entitySpawned.lifetime = lifetime;
	entitySpawned.width = boundingBox.getW();
	entitySpawned.height = boundingBox.getH();
}

void effect::SpawnAdjacentEntity::trigger(ComponentManager& compManager, EntityManager& entityManager, EventManager& eventManager, ComponentManager::EntityComponents* source, float x, float y)
{
	DrawState attackState = DrawState::none;

	StateComponent* state = source->getComponent<StateComponent>();
	PositionComponent* position = source->getComponent<PositionComponent>();
	GroupComponent* group = source->getComponent<GroupComponent>();
	CollisionComponent* collision = source->getComponent<CollisionComponent>();
	if (state && position && group)
	{
		entitySpawned.group = group->group;
		entitySpawned.hostileGroups = group->hostileGroups;
		entitySpawned.damageGroups = group->hostileGroups;

		// if entity has collision use its hitbox for placement, otherwise use sprite
		float localX, localY, width, height;
		if (collision)
		{
			localX = collision->boundingBox.getX() + position->x;
			localY = collision->boundingBox.getY() + position->y;
			width = collision->boundingBox.getW();
			height = collision->boundingBox.getH();
		}
		else
		{
			localX = position->x;
			localY = position->y;
			width = position->width;
			height = position->height;
		}

		switch (state->facing)
		{
		case Direction::down:
				localY += height;
				attackState = DrawState::attackDown;
				break;
		case Direction::up:
				localY -= height;
				attackState = DrawState::attackUp;
				break;
		case Direction::left:
				localX -= width;
				attackState = DrawState::attackLeft;
				break;
		case Direction::right:
				localX += width;
				attackState = DrawState::attackRight;
				break;
		}

		int attackId = entityManager.createEntity(localX, localY, entitySpawned, attackState, state->facing);
		eventManager.fireEvent<CollisionCheckEvent>(CollisionCheckEvent(attackId));
	}
}