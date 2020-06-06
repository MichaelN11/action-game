#pragma once

#include <vector>
#include "rectangle.h"
#include "system.h"

// forward declare
class EventManager;
class TileMap;

class CollisionSystem : public System
{
public:
	CollisionSystem(ComponentManager& compManager, EventManager& eventManager, const TileMap& tileMap);
	void afterUpdate();
private:
	EventManager& eventManager;
	const TileMap& tileMap;

	std::vector<CollisionComponent*> collisionList;

	void checkCollisions(int entityId);
	void checkTileCollisions(CollisionComponent* collisionComp, PositionComponent* positionComp);
	void checkEntityCollisions(CollisionComponent* collisionComp, PositionComponent* positionComp, ComponentManager::EntityComponents* entityComponents);
};