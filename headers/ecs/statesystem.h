#pragma once

#include "ecs/system.h"

class EventManager;
class ECS;

class StateSystem : public System
{
public:
	StateSystem(ComponentManager& compManager, EventManager& eventManager);
	void update(int deltaTime, ECS& ecs);
	static void setInvincible(ComponentManager::EntityComponents* entity, StateComponent* state, float baseTime);
	static void setStunned(ComponentManager::EntityComponents* entity, float baseTime);
	static void entityDeath(ComponentManager::EntityComponents* entity, StateComponent* state);
private:
	EventManager& eventManager;
};