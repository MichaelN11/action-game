#include "ecs/statesystem.h"
#include "eventmanager.h"
#include "ecs/entitymanager.h"

StateSystem::StateSystem(ComponentManager& compManager, EventManager& eventManager) : System(compManager), eventManager(eventManager)
{}

void StateSystem::setInvincible(ComponentManager::EntityComponents* entity, StateComponent* state, float baseTime)
{
	if (state->activityState != ActivityState::dead)
	{
		state->invincible = true;
		state->invincibilityTimer = (int)(baseTime * state->invTimeFactor);
		state->flashing = true;
		CollisionComponent* collision = entity->getComponent<CollisionComponent>();
		if (collision)
		{
			collision->collideWithEntities = false;
		}
	}
}

void StateSystem::setStunned(ComponentManager::EntityComponents* entity, float baseTime)
{
	StateComponent* targetState = entity->getComponent<StateComponent>();
	if (targetState->activityState != ActivityState::dead)
	{
		targetState->activityState = ActivityState::stunned;
		targetState->stunTimer = (int)(baseTime);
		targetState->previousDrawState = targetState->getBufferedDrawState();
		targetState->setDrawState(DrawState::stunned);
	}
}

void StateSystem::entityDeath(ComponentManager::EntityComponents* entity, StateComponent* state)
{
	state->setDrawState(DrawState::dead);
	state->activityState = ActivityState::dead;
	CollisionComponent* collision = entity->getComponent<CollisionComponent>();
	if (collision)
	{
		collision->solid = false;
		collision->interactable = false;
	}
	MovementComponent* movement = entity->getComponent<MovementComponent>();
	if (movement)
	{
		// if dead entity is moving and not decelerating, decelerate it to 0 within a second
		if (movement->dx > 0 && movement->xAcceleration >= 0)
		{
			movement->xAcceleration = (movement->dx / 1000) * -1;
		}
		else if (movement->dx < 0 && movement->xAcceleration <= 0)
		{
			movement->xAcceleration = (movement->dx / 1000) * -1;
		}

		if (movement->dy > 0 && movement->yAcceleration >= 0)
		{
			movement->yAcceleration = (movement->dy / 1000) * -1;
		}
		else if (movement->dy < 0 && movement->yAcceleration <= 0)
		{
			movement->yAcceleration = (movement->dy / 1000) * -1;
		}
	}
}

void StateSystem::update(int deltaTime, EntityManager& ecs)
{
	auto stateList = compManager.getComponentList<StateComponent>();
	for (auto state : stateList)
	{
		// small buffer when changing sprites, useful for things like diagonal movement
		if (state->getBufferedDrawState() != state->getDrawState())
		{
			if (state->drawBuffer >= state->drawBufferMax)
			{
				state->setDrawState(state->getBufferedDrawState());
			}
			else
			{
				state->drawBuffer += deltaTime;
			}
		}

		// recover from hitstun
		if (state->stunTimer > 0)
		{
			state->stunTimer -= deltaTime;
		}
		else if (state->activityState == ActivityState::stunned)
		{
			state->activityState = ActivityState::alive;
			DrawState newDrawState;
			switch (state->previousDrawState)
			{
				case DrawState::walkDown:
					newDrawState = DrawState::standDown;
					break;
				case DrawState::walkUp:
					newDrawState = DrawState::standUp;
					break;
				case DrawState::walkLeft:
					newDrawState = DrawState::standLeft;
					break;
				case DrawState::walkRight:
					newDrawState = DrawState::standRight;
					break;
				default:
					newDrawState = state->previousDrawState;
			}
			state->setDrawState(newDrawState);
			state->stunTimer = 0;
		}

		// invincibility after being hit
		if (state->invincibilityTimer > 0)
		{
			state->invincibilityTimer -= deltaTime;
		}
		else if (state->invincible == true)
		{
			state->invincible = false;
			state->invincibilityTimer = 0;
			state->flashing = false;

			CollisionComponent* collision = compManager.getComponent<CollisionComponent>(state->entityId);
			if (collision)
			{
				collision->collideWithEntities = true;
				eventManager.fireEvent<CollisionCheckEvent>(CollisionCheckEvent(collision->entityId));
			}
		}

		// removes dead entities after timer is up
		if (state->activityState == ActivityState::dead)
		{
			state->deathTimer += deltaTime;
			if (state->deathTimer >= state->timeToDie)
			{
				ecs.destroyEntity(state->entityId);
			}
		}

		// entity dies after lifetime is over
		if (state->activityState != ActivityState::dead && state->lifetime > 0)
		{
			state->lifeTimer += deltaTime;
			if (state->lifeTimer >= state->lifetime)
			{
				entityDeath(compManager.getEntityComponents(state->entityId), state.get());
			}
		}
	}
}