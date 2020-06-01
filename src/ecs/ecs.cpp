#include "ecs/ecs.h"
#include "graphics.h"
#include "eventmanager.h"
#include "config.h"

// temp
#include <cmath>

// DELETE LATER
#include <iostream>

ECS::ECS(EventManager& eventManager, const TileMap& tileMap) :
	eventManager(eventManager),
	playerSystem(compManager, eventManager),
	posUpdateSystem(compManager),
	spriteSystem(compManager),
	animationSystem(compManager),
	collisionSystem(compManager, eventManager, tileMap)
{

}

void ECS::draw(Graphics& graphics, const Rectangle<float>& view)
{
	spriteSystem.drawSprites(graphics, view);
}

void ECS::update(int deltaTime, const Rectangle<float>& view)
{
	posUpdateSystem.positionUpdate(deltaTime, view, eventManager);

	animationSystem.update(deltaTime);

	afterUpdate();
}

void ECS::afterUpdate()
{
	collisionSystem.afterUpdate();
}

std::pair<float, float> ECS::getPlayerPosition()
{
	auto playerList = compManager.getComponentList<PlayerComponent>();
	PositionComponent* playerPos = compManager.getComponent<PositionComponent>(playerList.at(0)->entityId);
	return std::make_pair(playerPos->x, playerPos->y);
}

void ECS::createEntityFromData(ComponentManager& compManager, int entityId, const EntityData& data)
{
	std::cout << "Entity # " << entityId << " created." << std::endl;

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
		compManager.addComponent(StateComponent(entityId, DrawState::standDown, 100));
		compManager.addComponent(AnimationComponent(entityId, data.animationTimeToUpdate, data.animationMap));
	}
	if (data.boundingBox.getW() > 0)
	{
		Rectangle<float> bBox = data.boundingBox;
		bBox.scalePositionAndSize(config::SPRITE_SCALE);
		compManager.addComponent(CollisionComponent(entityId, bBox, data.solid));
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
	PositionComponent position(entityId, x * config::SPRITE_SCALE, y * config::SPRITE_SCALE);
	compManager.addComponent(position);
	createEntityFromData(compManager, entityId, data);
}

void ECS::destroyEntity(int entityId)
{
	compManager.removeAllComponents(entityId);
	unusedEntityIds.push_back(entityId);

	std::cout << "Entity # " << entityId << " destroyed." << std::endl;
}

int ECS::getNextEntityId()
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

// sprite sheets need to be explicitly loaded with graphics class before they can be used

const EntityData ECS::PLAYER =
{
	// sprite file name path
	"content/tilesheets/link.png",
	// tile number on spritesheet
	1,
	// sprite width
	16,
	// sprite height
	16,
	// sprite layer
	2,
	// move speed
	0.2f,
	// is player
	true,
	// animation map
	&PlayerAnim::map,
	// animation time to update
	150,
	// bounding box
	Rectangle<float>(4.f, 4.f, 8.f, 12.f),
	// solid
	true
};

const EntityData ECS::DUMMY =
{
	// sprite file name path
	"content/tilesheets/tileset_legacy.png",
	// tile number on spritesheet
	32,
	// sprite width
	16,
	// sprite height
	16,
	// sprite layer
	1,
	// move speed
	0.1f,
	// is player
	false,
	// animation map
	nullptr,
	// animation time to update
	-1,
	// bounding box
	Rectangle<float>(0.f, 0.f, 16.f, 16.f),
	// solid
	true
};

std::unordered_map<DrawState, std::vector<AnimationFrame>> PlayerAnim::create_map()
{
	std::unordered_map<DrawState, std::vector<AnimationFrame>> map;
	map[DrawState::standDown] = std::vector<AnimationFrame>({ { 1, false, false, false } });
	map[DrawState::standUp] = std::vector<AnimationFrame>({ { 2, false, false, false } });
	map[DrawState::standLeft] = std::vector<AnimationFrame>({ { 3, false, false, false } });
	map[DrawState::standRight] = std::vector<AnimationFrame>({ { 3, false, true, false } });
	map[DrawState::walkDown] = std::vector<AnimationFrame>({ 
		{ 1, false, true, false },
		{ 1, false, false, false } });
	map[DrawState::walkUp] = std::vector<AnimationFrame>({
		{ 2, false, true, false },
		{ 2, false, false, false } });
	map[DrawState::walkLeft] = std::vector<AnimationFrame>({
		{ 4, false, false, false },
		{ 3, false, false, false } });
	map[DrawState::walkRight] = std::vector<AnimationFrame>({
		{ 4, false, true, false },
		{ 3, false, true, false } });
	return map;
}

std::unordered_map<DrawState, std::vector<AnimationFrame>> PlayerAnim::map = PlayerAnim::create_map();