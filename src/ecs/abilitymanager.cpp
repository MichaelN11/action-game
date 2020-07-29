#include "ecs/abilitymanager.h"
#include "ecs/statesystem.h"
#include "ability.h"

// DELETE LATER %%%%%%%%%%%%%%%%%%%%%%
#include <iostream>

AbilityManager::AbilityManager(ComponentManager& compManager, EventManager& eventManager, EntityManager& entityManager)
	: System(compManager), eventManager(eventManager), entityManager(entityManager)
{
	Ability swordAttack(compManager, entityManager, eventManager, "Sword Attack", effect::SWORD.lifetime);
	swordAttack.addEffect(std::unique_ptr<effect::AbilityEffect>(new effect::SpawnAdjacentEntity(effect::SWORD)));
	abilityMap.insert({ "swordAttack", std::move(swordAttack) });
}

void AbilityManager::useAbility(ComponentManager::EntityComponents* source, int abilityNum, float x, float y)
{
	std::cout << "got here" << std::endl;
	AbilityComponent* abilities = source->getComponent<AbilityComponent>();
	if (abilities)
	{
		auto it = abilities->abilityMap.find(abilityNum);
		if (it != abilities->abilityMap.end())
		{
			Ability* ability = it->second;
			std::cout << "ability activated" << std::endl;
			ability->activate(source, x, y);

			MovementComponent* movement = source->getComponent<MovementComponent>();
			if (movement)
			{
				movement->dx = 0;
				movement->dy = 0;
				movement->xAcceleration = 0;
				movement->yAcceleration = 0;
			}
			StateComponent* state = source->getComponent<StateComponent>();
			if (state)
			{
				DrawState attackState;
				switch (state->facing)
				{
				case Direction::down:
					attackState = DrawState::attackDown;
					break;
				case Direction::up:
					attackState = DrawState::attackUp;
					break;
				case Direction::left:
					attackState = DrawState::attackLeft;
					break;
				case Direction::right:
					attackState = DrawState::attackRight;
					break;
				}
				StateSystem::setStunned(source, (float)ability->getCastTime(), attackState);
			}
		}
	}
}