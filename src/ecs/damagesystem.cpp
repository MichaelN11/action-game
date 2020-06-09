#include "ecs/damagesystem.h"
#include "eventmanager.h"
#include <cmath>

// TEMPORARY DELETE LATER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <iostream>

DamageSystem::DamageSystem(ComponentManager& compManager, EventManager& eventManager) :
	System(compManager)
{
	eventManager.registerListener<DamageEvent>([this](DamageEvent dEvent) 
	{
		handleDamageEvent(dEvent.source, dEvent.target, dEvent.xDirection, dEvent.yDirection);
	});
}

void DamageSystem::handleDamageEvent(ComponentManager::EntityComponents* source, ComponentManager::EntityComponents* target, float xDirection, float yDirection)
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
						const float diagonalSpeed = 0.7071f;
						const float deceleration = 0.0004f;

						float xSpeed = sourceDamage->knockback;
						float ySpeed = sourceDamage->knockback;
						if (xDirection != 0 && yDirection != 0)
						{
							xSpeed *= diagonalSpeed;
							ySpeed *= diagonalSpeed;
						}

						targetMovement->dx = 0;
						targetMovement->dy = 0;

						if (xDirection != 0)
						{
							targetMovement->dx = copysign(xSpeed, xDirection);
							targetMovement->xAcceleration = copysign(deceleration, -xDirection);
						}
						if (yDirection != 0)
						{
							targetMovement->dy = copysign(ySpeed, yDirection);
							targetMovement->yAcceleration = copysign(deceleration, -yDirection);
						}

						std::cout << "dx: " << targetMovement->dx << "   dy: " << targetMovement->dy << "   xAcceleration: " << targetMovement->xAcceleration << "   yAcceleration: " << targetMovement->yAcceleration << std::endl;
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