#pragma once

#include "system.h"
#include "ability.h"

// forward declare
class EventManager;
class EntityManager;

class AbilityManager : public System
{
public:
	AbilityManager(ComponentManager& compManager, EventManager& eventManager, EntityManager& entityManager);

	static void useAbility(ComponentManager::EntityComponents* source, int abilityNum, float x, float y);

	std::unordered_map<std::string, Ability> abilityMap;

	static const AnimationSet SWORD;
private:
	EventManager& eventManager;
	EntityManager& entityManager;
};