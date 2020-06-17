#include "ecs/statesystem.h"
#include "eventmanager.h"

StateSystem::StateSystem(ComponentManager& compManager, EventManager& eventManager) : System(compManager), eventManager(eventManager)
{}

void StateSystem::update(int deltaTime)
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

			CollisionComponent* collision = compManager.getComponent<CollisionComponent>(state->entityId);
			if (collision)
			{
				collision->collideWithEntities = true;
				eventManager.fireEvent<CollisionCheckEvent>(CollisionCheckEvent(collision->entityId));
			}
		}
	}
}