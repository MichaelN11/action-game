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
	PositionUpdateSystem(ComponentManager& compManager);
	void positionUpdate(int deltaTime, const Rectangle<float>& view, EventManager& eventManager);
private:
	ComponentManager& compManager;
	const Rectangle<float>& view;

	// distance away from view where entities remain active
	static const int MAX_ACTIVE_DISTANCE;
};