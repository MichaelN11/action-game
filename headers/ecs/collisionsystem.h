#pragma once

#include <vector>
#include "rectangle.h"

// forward declare
class ComponentManager;
class EventManager;
class TileMap;
struct CollisionComponent;
struct PositionComponent;

class CollisionSystem
{
public:
	CollisionSystem(ComponentManager& compManager, EventManager& eventManager, const TileMap& tileMap);
private:
	ComponentManager& compManager;
	EventManager& eventManager;
	const TileMap& tileMap;

	void checkCollisions(int entityId);
	void checkTileCollisions(CollisionComponent* collisionComp, PositionComponent* positionComp);
	void checkEntityCollisions(CollisionComponent* collisionComp, PositionComponent* positionComp);
};