#pragma once

#include "abilityeffect.h"
#include "ecs/componentmanager.h"
class EntityManager;
class EventManager;

class Ability
{
public:
	Ability(ComponentManager& compManager, EntityManager& entityManager, EventManager& eventManager, std::string name, int castTime);

	void addEffect(std::unique_ptr<effect::AbilityEffect> effect);
	void activate(ComponentManager::EntityComponents* source, float x, float y);

	std::string getName() const;
	int getCastTime() const;

private:
	ComponentManager& compManager;
	EntityManager& entityManager;
	EventManager& eventManager;

	std::string name;
	int castTime;
	std::vector<std::unique_ptr<effect::AbilityEffect>> effectList;
};