#pragma once

#include "ecs/componentmanager.h"

namespace movement
{
	void standardMove(ComponentManager::EntityComponents* entity, float xMultiplier, float yMultiplier);
};