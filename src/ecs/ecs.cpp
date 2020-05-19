#include "ecs/ecs.h"
#include "graphics.h"
#include "eventmanager.h"

ECS::ECS(EventManager& eventManager) :
	playerSystem(compManager, eventManager),
	posUpdateSystem(compManager),
	spriteSystem(compManager)
{

}

void ECS::draw(Graphics& graphics)
{
	// TEMP CAMERA
	Rectangle view = graphics.getView();
	auto playerList = compManager.getComponentList<PlayerComponent>();
	PositionComponent* playerPos = compManager.getComponent<PositionComponent>(playerList.at(0)->entityId);
	graphics.setView((int)playerPos->x - view.getW() / 2, (int)playerPos->y - view.getH() / 2);

	spriteSystem.drawSprites(graphics);
}

void ECS::update(int deltaTime, const Rectangle& view)
{
	posUpdateSystem.positionUpdate(deltaTime, view);
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