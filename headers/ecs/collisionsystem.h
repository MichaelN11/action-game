#pragma once

#include <vector>
#include "rectangle.h"

// forward declare
class ComponentManager;
class EventManager;
class TileMap;

class CollisionSystem
{
public:
	CollisionSystem(ComponentManager& compManager, EventManager& eventManager, const TileMap& tileMap);
private:
	ComponentManager& compManager;
	void checkSolidCollisions(int entityId);
	const TileMap& tileMap;
};