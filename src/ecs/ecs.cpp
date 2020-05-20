#include "ecs/ecs.h"
#include "graphics.h"
#include "eventmanager.h"

// temp
#include <cmath>

ECS::ECS(EventManager& eventManager) :
	playerSystem(compManager, eventManager),
	posUpdateSystem(compManager),
	spriteSystem(compManager)
{

}

void ECS::draw(Graphics& graphics, const Rectangle<float>& view)
{
	spriteSystem.drawSprites(graphics, view);
}

void ECS::update(int deltaTime, const Rectangle<float>& view)
{
	posUpdateSystem.positionUpdate(deltaTime, view);
}

std::pair<float, float> ECS::getPlayerPosition()
{
	auto playerList = compManager.getComponentList<PlayerComponent>();
	PositionComponent* playerPos = compManager.getComponent<PositionComponent>(playerList.at(0)->entityId);
	return std::make_pair(playerPos->x, playerPos->y);
}

void ECS::createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data)
{
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
}

void ECS::createEntity(const EntityData& data)
{
	int entityId = getNextEntityId();
	createEntityFromData(compManager, entityId, data);
}

void ECS::createEntity(float x, float y, const EntityData& data)
{
	int entityId = getNextEntityId();
	PositionComponent position(entityId, x, y);
	compManager.addComponent(position);
	createEntityFromData(compManager, entityId, data);
}

int ECS::getNextEntityId()
{
	return nextEntityId++;
}

// sprite sheets need to be explicitly loaded with graphics class before they can be used

const EntityData ECS::PLAYER =
{
	// sprite file name path
	"content/tilesheets/1bit.png",
	// tile number on spritesheet
	25,
	// sprite width
	16,
	// sprite height
	16,
	// sprite layer
	2,
	// move speed
	0.1f,
	// is player
	true
};

const EntityData ECS::DUMMY =
{
	// sprite file name path
	"content/tilesheets/1bit.png",
	// tile number on spritesheet
	26,
	// sprite width
	16,
	// sprite height
	16,
	// sprite layer
	1,
	// move speed
	0.f,
	// is player
	false
};