#include "entitymanager.h"

// sprite sheets need to be explicitly loaded with graphics class before they can be used

const EntityData EntityManager::PLAYER =
{
	// sprite file name path
	"content/tilesheets/1bit.png",
	// tile number on spritesheet
	25,
	// sprite width
	16,
	// sprite height
	16,
	// move speed
	0.3,
	// is player
	true
};

const EntityData EntityManager::DUMMY =
{
	// sprite file name path
	"content/tilesheets/1bit.png",
	// tile number on spritesheet
	26,
	// sprite width
	16,
	// sprite height
	16,
	// move speed
	0,
	// is player
	false
};

void EntityManager::createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data)
{
	if (data.spritePath != "")
	{
		compManager.addComponent(SpriteComponent(entityId, data.spritePath, data.tileNum, data.spriteWidth, data.spriteHeight));
	}
	if (data.moveSpeed >= 0)
	{
		compManager.addComponent(MovementComponent(entityId, 0.f, 0.f, data.moveSpeed));
	}
	if (data.player)
	{
		compManager.addComponent(PlayerComponent(entityId));
	}
}

void EntityManager::createEntity(ComponentManager& compManager, const EntityData& data)
{
	int entityId = getNextId();
	createEntityFromData(compManager, entityId, data);
}

void EntityManager::createEntity(ComponentManager& compManager, float x, float y, const EntityData& data)
{
	int entityId = getNextId();
	PositionComponent position(entityId, x, y);
	compManager.addComponent(position);
	createEntityFromData(compManager, entityId, data);
}

int EntityManager::getNextId()
{
	return nextId++;
}