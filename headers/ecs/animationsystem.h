#pragma once

// forward declare
class ComponentManager;

class AnimationSystem
{
public:
	AnimationSystem(ComponentManager& compManager);
	void update(int deltaTime);
private:
	ComponentManager& compManager;
};