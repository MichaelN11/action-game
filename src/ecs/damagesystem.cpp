#include "ecs/damagesystem.h"
#include "eventmanager.h"

// TEMPORARY DELETE LATER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <iostream>

DamageSystem::DamageSystem(ComponentManager& compManager, EventManager& eventManager) :
	System(compManager)
{
	eventManager.registerListener<DamageEvent>([this](DamageEvent dEvent) 
	{
		handleDamageEvent(dEvent.source, dEvent.target);
	});
}

void DamageSystem::handleDamageEvent(ComponentManager::EntityComponents* source, ComponentManager::EntityComponents* target)
{
	StateComponent* targetState = target->getComponent<StateComponent>();
	if (targetState && targetState->invincible == false)
	{
		DamageComponent* sourceDamage = source->getComponent<DamageComponent>();
		if (sourceDamage)
		{
			HealthComponent* targetHealth = target->getComponent<HealthComponent>();
			if (targetHealth)
			{
				bool hostile = false;
				GroupComponent* targetGroup = target->getComponent<GroupComponent>();
				if (targetGroup)
				{
					for (Group group : sourceDamage->damageGroups)
					{
						if (group == targetGroup->group)
						{
							hostile = true;
							break;
						}
					}
				}
				else
				{
					hostile = true;
				}

				if (hostile)
				{
					int damage = sourceDamage->damage;
					dealDamage(source, targetHealth, damage);

					targetState->activityState = ActivityState::stunned;
					targetState->stunTimer = targetState->maxStunTime;
					targetState->previousDrawState = targetState->getBufferedDrawState();
					targetState->setDrawState(DrawState::stunned);
					targetState->invincible = true;
					targetState->invincibilityTimer = targetState->maxInvTime;

					MovementComponent* targetMovement = target->getComponent<MovementComponent>();
					if (targetMovement)
					{
						targetMovement->dx = 0;
						targetMovement->dy = 0;
					}
				}
			}
		}
	}
}

void DamageSystem::dealDamage(ComponentManager::EntityComponents* source, HealthComponent* health, int damage)
{
	if (source && health)
	{
		health->health -= damage;
		std::cout << "Entity " << health->entityId << " took " << damage << " damage! Health remaining: " << health->health << std::endl;
	}
}