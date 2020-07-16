#include "ecs/damagesystem.h"
#include "ecs/statesystem.h"
#include "eventmanager.h"
#include "ecs/movement.h"
#include <cmath>

// TEMPORARY DELETE LATER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <iostream>

DamageSystem::DamageSystem(ComponentManager& compManager, EventManager& eventManager) :
	System(compManager)
{
	eventManager.registerListener<DamageEvent>([this](DamageEvent dEvent) 
	{
		handleDamageEvent(dEvent.source, dEvent.target, dEvent.collisionAngle);
	});
}

void DamageSystem::handleDamageEvent(ComponentManager::EntityComponents* source, ComponentManager::EntityComponents* target, float collisionAngle)
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
					dealDamage(target, targetHealth, targetState, damage);

					// Force knockback to occur in direction of the damage, add PI to reverse angle if its wrong direction
					Direction dir = sourceDamage->direction;
					if ((dir == Direction::up && collisionAngle < 0) ||
						(dir == Direction::down && collisionAngle > 0) ||
						(dir == Direction::left && (collisionAngle < PI / 2 && collisionAngle > -PI / 2)) ||
						(dir == Direction::right && (collisionAngle > PI / 2 || collisionAngle < -PI / 2)))
					{
						collisionAngle += PI;
					}

					float knockbackSpeed = sourceDamage->knockback;
					float knockbackDeceleration = sourceDamage->knockbackDeceleration;
					movement::knockback(target, collisionAngle, knockbackSpeed, knockbackDeceleration);
					StateSystem::setInvincible(target, targetState, knockbackSpeed / knockbackDeceleration);
				}
			}
		}
	}
}

void DamageSystem::dealDamage(ComponentManager::EntityComponents* target, HealthComponent* health, StateComponent* state, int damage)
{
	if (target && health && state)
	{
		health->health -= damage;
		std::cout << "Entity " << health->entityId << " took " << damage << " damage! Health remaining: " << health->health << std::endl;

		//dead
		if (health->health <= 0)
		{
			StateSystem::entityDeath(target, state);
		}
	}
}