#pragma once

#include "ecs/system.h"

class EventManager;
class ECS;

class StateSystem : public System
{
public:
	StateSystem(ComponentManager& compManager, EventManager& eventManager);
	void update(int deltaTime, ECS& ecs);
private:
	EventManager& eventManager;
};