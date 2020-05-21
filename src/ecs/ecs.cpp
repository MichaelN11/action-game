#include "ecs/ecs.h"
#include "graphics.h"
#include "eventmanager.h"

// temp
#include <cmath>

ECS::ECS(EventManager& eventManager) :
	playerSystem(compManager, eventManager),
	posUpdateSystem(compManager),
	spriteSystem(compManager),
	animationSystem(compManager)
{

}

void ECS::draw(Graphics& graphics, const Rectangle<float>& view)
{
	spriteSystem.drawSprites(graphics, view);
}

void ECS::update(int deltaTime, const Rectangle<float>& view)
{
	posUpdateSystem.positionUpdate(deltaTime, view);

	animationSystem.update(deltaTime);
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
	if (data.animationMap)
	{
		compManager.addComponent(StateComponent(entityId, DrawState::walkLeft));
		compManager.addComponent(AnimationComponent(entityId, data.animationTimeToUpdate, data.animationMap));
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
	0.1f,
	// is player
	true,
	// animation map
	&PlayerAnim::map,
	// animation time to update
	150
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
	false,
	// animation map
	nullptr,
	// animation time to update
	-1
};

std::unordered_map<DrawState, std::vector<AnimationFrame>> PlayerAnim::create_map()
{
	std::unordered_map<DrawState, std::vector<AnimationFrame>> map;
	map[DrawState::standDown] = std::vector<AnimationFrame>({ { 1, false, false, false } });
	map[DrawState::standUp] = std::vector<AnimationFrame>({ { 2, false, false, false } });
	map[DrawState::standLeft] = std::vector<AnimationFrame>({ { 3, false, false, false } });
	map[DrawState::standRight] = std::vector<AnimationFrame>({ { 3, false, true, false } });
	map[DrawState::walkDown] = std::vector<AnimationFrame>({ 
		{ 1, false, false, false },
		{ 1, false, true, false } });
	map[DrawState::walkUp] = std::vector<AnimationFrame>({
		{ 2, false, false, false },
		{ 2, false, true, false } });
	map[DrawState::walkLeft] = std::vector<AnimationFrame>({
		{ 3, false, false, false },
		{ 4, false, false, false } });
	map[DrawState::walkRight] = std::vector<AnimationFrame>({
		{ 3, false, true, false },
		{ 4, false, true, false } });
	return map;
}

std::unordered_map<DrawState, std::vector<AnimationFrame>> PlayerAnim::map = PlayerAnim::create_map();