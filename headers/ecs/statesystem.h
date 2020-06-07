#pragma once

#include "ecs/system.h"

class StateSystem : public System
{
public:
	StateSystem(ComponentManager& commpManager);
	void update(int deltaTime);
};