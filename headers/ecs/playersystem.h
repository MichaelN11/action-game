#pragma once

#include <unordered_map>
#include "enums.h"

//forward declare
class ComponentManager;
class EventManager;

class PlayerSystem
{
public:
	PlayerSystem(ComponentManager& compManager, EventManager& eventManager);
private:
	ComponentManager& compManager;
	void parseMovement(const std::unordered_map<Keybind, bool>& heldKeys);
	void updateMovement(float xDirection, float yDirection);

	static const float DIAGONAL_SPEED;
};