#pragma once

#include "system.h"

class AnimationSystem : public System
{
public:
	AnimationSystem(ComponentManager& compManager);
	void update(int deltaTime);
};