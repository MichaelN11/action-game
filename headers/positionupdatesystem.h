#pragma once

#include "rectangle.h"

// forward declare
class ComponentManager;
class EventManager;
/*
	PositionUpdateSystem class
	updates position components based on movement components
	also checks positions to make entities inactive
*/

class PositionUpdateSystem
{
public:
	PositionUpdateSystem(ComponentManager& compManager, EventManager& eventManager, const Rectangle& view);
	void positionUpdate(int deltaTime);
private:
	ComponentManager& compManager;
	Rectangle activeBounds;

	// distance away from view where entities remain active
	static const int MAX_ACTIVE_DISTANCE;
};