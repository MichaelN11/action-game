#pragma once

#include <unordered_map>
#include "enums.h"
#include "system.h"

// forward declare
class EventManager;
class MovementSystem;

class PlayerSystem : public System
{
public:
	PlayerSystem(ComponentManager& compManager, EventManager& eventManager);
private:
	void parseMovement(const std::unordered_map<Keybind, bool>& heldKeys);
	void updateMovement(float xDirection, float yDirection);

	static const float DIAGONAL_SPEED;
};