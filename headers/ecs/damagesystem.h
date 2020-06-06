#pragma once

#include "system.h"

// forward declare
class EventManager;

class DamageSystem : public System
{
public:
	DamageSystem(ComponentManager& compManager, EventManager& eventManager);
	void handleDamageEvent(ComponentManager::EntityComponents* source, ComponentManager::EntityComponents* target);
	void dealDamage(ComponentManager::EntityComponents* source, HealthComponent* health, int damage);
};