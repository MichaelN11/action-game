#include "ecs/ecs.h"
#include "graphics.h"
#include "eventmanager.h"
#include "config.h"

// temp
#include <cmath>

// DELETE LATER
//#include <iostream>

ECS::ECS(EventManager& eventManager, const TileMap& tileMap) :
	entityManager(compManager),
	eventManager(eventManager),
	playerSystem(compManager, eventManager, entityManager),
	posUpdateSystem(compManager),
	spriteSystem(compManager),
	animationSystem(compManager),
	collisionSystem(compManager, eventManager, tileMap),
	damageSystem(compManager, eventManager),
	stateSystem(compManager, eventManager)
{

}

EntityManager& ECS::getEntityManager()
{
	return entityManager;
}

void ECS::draw(Graphics& graphics, const Rectangle<float>& view)
{
	spriteSystem.drawSprites(graphics, view);
}

void ECS::update(int deltaTime, const Rectangle<float>& view)
{
	posUpdateSystem.positionUpdate(deltaTime, view, eventManager);

	stateSystem.update(deltaTime, entityManager);
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
	if (playerList.size() > 0)
	{
		PositionComponent* playerPos = compManager.getComponent<PositionComponent>(playerList.at(0)->entityId);
		return std::make_pair(playerPos->x, playerPos->y);
	}
	else 
		return std::make_pair(0.f, 0.f);
}

// sprite sheets need to be explicitly loaded with graphics class before they can be used



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
	0.05f,
	// is player
	false,
	// animation map
	nullptr,
	// animation time to update
	-1,
	// bounding box
	Rectangle<float>(0.f, 0.f, 16.f, 16.f),
	// solid
	true,
	// interactable
	true,
	// health
	100,
	// damage
	10,
	// damage group
	{ Group::player },
	// group
	Group::enemy,
	// hostile groups
	{ Group::player },
	// lifetime
	-1
};

std::unordered_map<DrawState, Animation> createPlayerAnims()
{
	std::unordered_map<DrawState, Animation> map;
	map[DrawState::standDown] = Animation( std::vector<AnimationFrame>({ AnimationFrame(1, false, false, false ) }), true );
	map[DrawState::standUp] = Animation( std::vector<AnimationFrame>({ AnimationFrame(2, false, false, false ) }), true );
	map[DrawState::standLeft] = Animation( std::vector<AnimationFrame>({ AnimationFrame(3, false, false, false ) }), true );
	map[DrawState::standRight] = Animation( std::vector<AnimationFrame>({ AnimationFrame(3, false, true, false ) }), true );
	map[DrawState::walkDown] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(1, false, true, false ),
		AnimationFrame(1, false, false, false ) }), true );
	map[DrawState::walkUp] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(2, false, true, false ),
		AnimationFrame(2, false, false, false ) }), true );
	map[DrawState::walkLeft] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(4, false, false, false ),
		AnimationFrame(3, false, false, false ) }), true );
	map[DrawState::walkRight] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(4, false, true, false ),
		AnimationFrame(3, false, true, false ) }), true );
	map[DrawState::stunned] = Animation( std::vector<AnimationFrame>({ AnimationFrame(91, false, false, false ) }), true );
	map[DrawState::dead] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(92, false, false, false ),
		AnimationFrame(93, false, false, false ),
		AnimationFrame(94, false, false, false ),
		AnimationFrame(95, false, false, false ) }), false );
	map[DrawState::attackDown] = Animation( std::vector<AnimationFrame>({ AnimationFrame(112, false, false, false ) }), true );
	map[DrawState::attackUp] = Animation( std::vector<AnimationFrame>({ AnimationFrame(110, false, false, false ) }), true );
	map[DrawState::attackLeft] = Animation( std::vector<AnimationFrame>({ AnimationFrame(113, false, false, false ) }), true );
	map[DrawState::attackRight] = Animation( std::vector<AnimationFrame>({ AnimationFrame(113, false, true, false ) }), true );
	return map;
}

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
	std::make_unique<std::unordered_map<DrawState, Animation>>(createPlayerAnims()),
	// animation time to update
	150,
	// bounding box
	Rectangle<float>(4.f, 4.f, 8.f, 12.f),
	// solid
	true,
	// interactable
	true,
	// health
	100,
	// damage
	0,
	// damage group
	//{ },
	{ Group::enemy },
	// group
	Group::player,
	// hostile groups
	{ Group::enemy },
	// lifetime
	-1
};