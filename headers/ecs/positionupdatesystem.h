#pragma once

#include "rectangle.h"
#include "system.h"

// forward declare
class EventManager;
/*
	PositionUpdateSystem class
	updates position components based on movement components
	also checks positions to make entities inactive
*/

class PositionUpdateSystem : public System
{
public:
	PositionUpdateSystem(ComponentManager& compManager);
	void positionUpdate(int deltaTime, const Rectangle<float>& view, EventManager& eventManager);
private:
	const Rectangle<float>& view;
	void applyHalfAcceleration(MovementComponent* movement, int deltaTime);

	// distance away from view where entities remain active
	static const int MAX_ACTIVE_DISTANCE;
};