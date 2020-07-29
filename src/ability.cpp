#include "ability.h"

Ability::Ability(ComponentManager& compManager, EntityManager& entityManager, EventManager& eventManager, std::string name, int castTime) 
	: compManager(compManager), entityManager(entityManager), eventManager(eventManager), name(name), castTime(castTime)
{}

void Ability::addEffect(std::unique_ptr<effect::AbilityEffect> effect)
{
	effectList.push_back(std::move(effect));
}

void Ability::activate(ComponentManager::EntityComponents* source, float x, float y)
{
	for (auto& effect : effectList)
	{
		effect.get()->trigger(compManager, entityManager, eventManager, source, x, y);
	}
}

std::string Ability::getName() const
{
	return name;
}

int Ability::getCastTime() const
{
	return castTime;
}