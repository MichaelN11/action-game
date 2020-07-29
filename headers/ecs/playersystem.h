#pragma once

//#include <unordered_map>
#include "enums.h"
#include "system.h"
#include "entitymanager.h"

// forward declare
class EventManager;

class PlayerSystem : public System
{
public:
	PlayerSystem(ComponentManager& compManager, EventManager& eventManager, EntityManager& entityManager);
private:
	EntityManager& entityManager;
	EventManager& eventManager;

	void parseMovement(const std::unordered_map<Keybind, bool>&);
	void updateMovement(float xDirection, float yDirection);
	void attack();

	static const float DIAGONAL_SPEED;
	static const EntityData SWORD;
};