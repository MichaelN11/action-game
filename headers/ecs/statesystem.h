#pragma once

#include "ecs/system.h"

class StateSystem : public System
{
public:
	StateSystem(ComponentManager& compManager);
	void update(int deltaTime);
};