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
	if (state && position && group)
	{
		entitySpawned.group = group->group;
		entitySpawned.hostileGroups = group->hostileGroups;
		entitySpawned.damageGroups = group->hostileGroups;

		float localX = position->x;
		float localY = position->y;

		switch (state->facing)
		{
		case Direction::down:
				localY += position->height;
				attackState = DrawState::attackDown;
				break;
		case Direction::up:
				localY -= position->height;
				attackState = DrawState::attackUp;
				break;
		case Direction::left:
				localX -= position->width;
				attackState = DrawState::attackLeft;
				break;
		case Direction::right:
				localX += position->width;
				attackState = DrawState::attackRight;
				break;
		}

		int attackId = entityManager.createEntity(localX, localY, entitySpawned, attackState, state->facing);
		eventManager.fireEvent<CollisionCheckEvent>(CollisionCheckEvent(attackId));
	}
}