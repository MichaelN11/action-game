#include "ecs/ecs.h"
#include "graphics.h"
#include "eventmanager.h"
#include "config.h"

// temp
#include <cmath>

// DELETE LATER
//#include <iostream>

ECS::ECS(EventManager& eventManager, const TileMap& tileMap) :
	entityManager(compManager, eventManager),
	eventManager(eventManager),
	playerSystem(compManager, eventManager, entityManager),
	posUpdateSystem(compManager),
	spriteSystem(compManager),
	animationSystem(compManager),
	collisionSystem(compManager, eventManager, tileMap),
	damageSystem(compManager, eventManager),
	stateSystem(compManager, eventManager),
	aiSystem(compManager, tileMap)
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
	aiSystem.update(deltaTime);

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
	// animation set
	{
		// animation map
		std::unordered_map<DrawState, Animation>(),
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
		// animation time to update
		-1
	},
	// move speed
	0.05f,
	// is player
	false,
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
	-1,
	// entity width
	16.f,
	// entity height
	16.f,
	// ability list
	{ },
	// ai behavior
	Behavior::none
};

std::unordered_map<DrawState, Animation> createPlayerAnims()
{
	std::unordered_map<DrawState, Animation> map;
	map[DrawState::standDown] = Animation( std::vector<AnimationFrame>({ AnimationFrame(1, false, false, false ) }), true );
	map[DrawState::standUp] = Animation( std::vector<AnimationFrame>({ AnimationFrame(16, false, false, false ) }), true );
	map[DrawState::standLeft] = Animation( std::vector<AnimationFrame>({ AnimationFrame(11, false, false, false ) }), true );
	map[DrawState::standRight] = Animation( std::vector<AnimationFrame>({ AnimationFrame(6, false, false, false ) }), true );
	map[DrawState::walkDown] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(1, false, false, false),
		AnimationFrame(2, false, false, false),
		AnimationFrame(3, false, false, false),
		AnimationFrame(4, false, false, false ) }), true );
	map[DrawState::walkUp] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(16, false, false, false),
		AnimationFrame(17, false, false, false),
		AnimationFrame(18, false, false, false),
		AnimationFrame(19, false, false, false ) }), true );
	map[DrawState::walkLeft] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(11, false, false, false),
		AnimationFrame(12, false, false, false),
		AnimationFrame(13, false, false, false),
		AnimationFrame(14, false, false, false ) }), true );
	map[DrawState::walkRight] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(6, false, false, false),
		AnimationFrame(7, false, false, false ),
		AnimationFrame(8, false, false, false),
		AnimationFrame(9, false, false, false ) }), true );
	map[DrawState::stunned] = Animation( std::vector<AnimationFrame>({ AnimationFrame(43, false, false, false ) }), true );
	map[DrawState::dead] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(41, false, false, false ),
		AnimationFrame(42, false, false, false ) }), false );
	map[DrawState::attackDown] = Animation( std::vector<AnimationFrame>({
		AnimationFrame(21, false, false, false),
		AnimationFrame(22, false, false, false),
		AnimationFrame(23, false, false, false),
		AnimationFrame(24, false, false, false ) }), false );
	map[DrawState::attackUp] = Animation( std::vector<AnimationFrame>({ 
		AnimationFrame(36, false, false, false),
		AnimationFrame(37, false, false, false),
		AnimationFrame(38, false, false, false),
		AnimationFrame(39, false, false, false ) }), false);
	map[DrawState::attackLeft] = Animation( std::vector<AnimationFrame>({ 
		AnimationFrame(31, false, false, false),
		AnimationFrame(32, false, false, false),
		AnimationFrame(33, false, false, false),
		AnimationFrame(34, false, false, false ) }), false);
	map[DrawState::attackRight] = Animation( std::vector<AnimationFrame>({ 
		AnimationFrame(26, false, false, false),
		AnimationFrame(27, false, false, false),
		AnimationFrame(28, false, false, false),
		AnimationFrame(29, false, true, false ) }), false);
	return map;
}

const EntityData ECS::PLAYER =
{
	// Animation Set
	{
		// animation map
		createPlayerAnims(),
		// sprite file name path
		"content/tilesheets/maincharacter.png",
		// tile number on spritesheet
		1,
		// sprite width
		32,
		// sprite height
		32,
		// sprite layer
		2,
		// animation time to update
		150
	},
	// move speed
	0.1f,
	// is player
	true,
	// bounding box
	Rectangle<float>(8.f, 11.f, 15.f, 16.f),
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
	-1,
	// entity width
	32.f,
	// entity height
	32.f,
	// ability list
	{ "swordAttack" },
	// ai behavior
	Behavior::none
};

std::unordered_map<DrawState, Animation> createEnemyAnims()
{
	std::unordered_map<DrawState, Animation> map;
	map[DrawState::standDown] = Animation(std::vector<AnimationFrame>({ AnimationFrame(11, false, false, false) }), true);
	map[DrawState::standUp] = Animation(std::vector<AnimationFrame>({ AnimationFrame(8, false, false, false) }), true);
	map[DrawState::standLeft] = Animation(std::vector<AnimationFrame>({ AnimationFrame(4, false, false, false) }), true);
	map[DrawState::standRight] = Animation(std::vector<AnimationFrame>({ AnimationFrame(2, false, false, false) }), true);
	map[DrawState::walkDown] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(10, false, false, false),
		AnimationFrame(11, false, false, false) }), true);
	map[DrawState::walkUp] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(7, false, false, false),
		AnimationFrame(8, false, false, false) }), true);
	map[DrawState::walkLeft] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(5, false, false, false),
		AnimationFrame(4, false, false, false) }), true);
	map[DrawState::walkRight] = Animation(std::vector<AnimationFrame>({
		AnimationFrame(1, false, false, false),
		AnimationFrame(2, false, false, false) }), true);
	//map[DrawState::stunned] = Animation(std::vector<AnimationFrame>({ AnimationFrame(91, false, false, false) }), true);
	//map[DrawState::dead] = Animation(std::vector<AnimationFrame>({
	//	AnimationFrame(92, false, false, false),
	//	AnimationFrame(93, false, false, false),
	//	AnimationFrame(94, false, false, false),
	//	AnimationFrame(95, false, false, false) }), false);
	//map[DrawState::attackDown] = Animation(std::vector<AnimationFrame>({ AnimationFrame(112, false, false, false) }), true);
	//map[DrawState::attackUp] = Animation(std::vector<AnimationFrame>({ AnimationFrame(110, false, false, false) }), true);
	//map[DrawState::attackLeft] = Animation(std::vector<AnimationFrame>({ AnimationFrame(113, false, false, false) }), true);
	//map[DrawState::attackRight] = Animation(std::vector<AnimationFrame>({ AnimationFrame(113, false, true, false) }), true);
	return map;
}

const EntityData ECS::ENEMY =
{
	// Animation Set
	{
		// animation map
		createEnemyAnims(),
		// sprite file name path
		"content/tilesheets/enemies.png",
		// tile number on spritesheet
		1,
		// sprite width
		16,
		// sprite height
		16,
		// sprite layer
		1,
		// animation time to update
		150
	},
	// move speed
	0.05f,
	// is player
	false,
	// bounding box
	Rectangle<float>(0.f, 0.f, 16.f, 16.f),
	// solid
	true,
	// interactable
	true,
	// health
	50,
	// damage
	10,
	// damage group
	//{ },
	{ Group::player },
	// group
	Group::enemy,
	// hostile groups
	{ Group::player },
	// lifetime
	-1,
	// entity width
	16.f,
	// entity height
	16.f,
	// ability list
	{ "swordAttack" },
	// ai behavior
	Behavior::wander
};