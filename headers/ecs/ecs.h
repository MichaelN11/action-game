#pragma once
#include "ecs/componentmanager.h"
#include "ecs/playersystem.h"
#include "ecs/positionupdatesystem.h"
#include "ecs/spritesystem.h"
#include "ecs/animationsystem.h"
#include "ecs/collisionsystem.h"
#include "ecs/damagesystem.h"
#include "ecs/statesystem.h"
#include "ecs/entitymanager.h"
#include "ecs/aisystem.h"

// forward declare
class Graphics;
class EventManager;
class TileMap;

class ECS
{
public:
	ECS(EventManager& eventManager, const TileMap& tileMap);
	void draw(Graphics& graphics, const Rectangle<float>& view);
	void update(int deltaTime, const Rectangle<float>& view);
	void afterUpdate();
	EntityManager& getEntityManager();

	std::pair<float, float> getPlayerPosition();

	// mainly for testing
	//ComponentManager& getCompManager()
	//{
	//	return compManager;
	//}

	static const EntityData PLAYER;
	static const EntityData DUMMY;
	static const EntityData ENEMY;
private:
	EventManager& eventManager;
	ComponentManager compManager;
	EntityManager entityManager;
	PlayerSystem playerSystem;
	PositionUpdateSystem posUpdateSystem;
	SpriteSystem spriteSystem;
	AnimationSystem animationSystem;
	CollisionSystem collisionSystem;
	DamageSystem damageSystem;
	StateSystem stateSystem;
	AISystem aiSystem;
};