#pragma once
#include "ecs/componentmanager.h"

class System
{
public:
	System(ComponentManager& compManager) : compManager(compManager)
	{}
protected:
	ComponentManager& compManager;
};