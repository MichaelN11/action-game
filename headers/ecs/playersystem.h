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
	void setMovement(const std::unordered_map<Keybind, bool>& heldKeys);
	void setMovespeed(float xDirection, float yDirection);

	static const float DIAGONAL_SPEED;
};