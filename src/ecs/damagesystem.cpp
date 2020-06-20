#include "ecs/damagesystem.h"
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
					dealDamage(source, targetHealth, damage);

					float knockbackSpeed = sourceDamage->knockback;
					float knockbackDeceleration = sourceDamage->knockbackDeceleration;

					movement::knockback(target, collisionAngle, knockbackSpeed, knockbackDeceleration);

					targetState->invincible = true;
					targetState->invincibilityTimer = (int)((knockbackSpeed / knockbackDeceleration) * targetState->invTimeFactor);
					CollisionComponent* collision = target->getComponent<CollisionComponent>();
					if (collision)
					{
						collision->collideWithEntities = false;
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