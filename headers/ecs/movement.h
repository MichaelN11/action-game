#pragma once

#include "ecs/componentmanager.h"

namespace movement
{
	void standardMove(ComponentManager::EntityComponents* entity, float xMultiplier, float yMultiplier);

	void knockback(ComponentManager::EntityComponents* entity, float xDirection, float yDirection, float speed, float deceleration);

	const float DIAGONAL_SPEED = 0.7071f;
	//const float DIAGONAL_SPEED = 1.f;
};