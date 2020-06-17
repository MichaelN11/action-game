#pragma once

#include "ecs/system.h"

class EventManager;

class StateSystem : public System
{
public:
	StateSystem(ComponentManager& compManager, EventManager& eventManager);
	void update(int deltaTime);
private:
	EventManager& eventManager;
};