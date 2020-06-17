#include "ecs/movement.h"

void movement::standardMove(ComponentManager::EntityComponents* entity, float xMultiplier, float yMultiplier)
{
	MovementComponent* movement = entity->getComponent<MovementComponent>();
	if (movement)
	{
		StateComponent* state = entity->getComponent<StateComponent>();
		if (state)
		{
			// only move if active
			if (state->activityState == ActivityState::alive)
			{
				bool standing = (state->getBufferedDrawState() == DrawState::standDown ||
					state->getBufferedDrawState() == DrawState::standLeft ||
					state->getBufferedDrawState() == DrawState::standUp ||
					state->getBufferedDrawState() == DrawState::standRight);

				if (yMultiplier == 0 && xMultiplier == 0)
				{
					switch (state->getDrawState())
					{
					case DrawState::walkRight:
						state->setDrawState(DrawState::standRight);
						break;
					case DrawState::walkLeft:
						state->setDrawState(DrawState::standLeft);
						break;
					case DrawState::walkUp:
						state->setDrawState(DrawState::standUp);
						break;
					default:
						state->setDrawState(DrawState::standDown);
					}
				}
				// if standing change to walking immediately, if walking, buffer the change
				else if (yMultiplier == 0)
				{
					if (xMultiplier > 0)
					{
						if (standing)
							state->setDrawState(DrawState::walkRight);
						else
							state->setBufferedDrawState(DrawState::walkRight);
					}
					else if (xMultiplier < 0)
					{
						if (standing)
							state->setDrawState(DrawState::walkLeft);
						else
							state->setBufferedDrawState(DrawState::walkLeft);
					}
				}
				else if (xMultiplier == 0)
				{
					if (yMultiplier > 0)
					{
						if (standing)
							state->setDrawState(DrawState::walkDown);
						else
							state->setBufferedDrawState(DrawState::walkDown);
					}
					else if (yMultiplier < 0)
					{
						if (standing)
							state->setDrawState(DrawState::walkUp);
						else
							state->setBufferedDrawState(DrawState::walkUp);
					}
				}
				else if (standing)
				{
					if (yMultiplier > 0)
						state->setDrawState(DrawState::walkDown);
					else
						state->setDrawState(DrawState::walkUp);
				}

				movement->dx = movement->moveSpeed * xMultiplier;
				movement->dy = movement->moveSpeed * yMultiplier;
			}
		}
		// if no state component, then it just moves
		else
		{
			movement->dx = movement->moveSpeed * xMultiplier;
			movement->dy = movement->moveSpeed * yMultiplier;
		}
	}
}

void movement::knockback(ComponentManager::EntityComponents* entity, float xDirection, float yDirection, float speed, float deceleration)
{
	MovementComponent* targetMovement = entity->getComponent<MovementComponent>();
	if (targetMovement)
	{
		StateComponent* targetState = entity->getComponent<StateComponent>();
		targetState->activityState = ActivityState::stunned;
		targetState->stunTimer = (int)(speed / deceleration);
		targetState->previousDrawState = targetState->getBufferedDrawState();
		targetState->setDrawState(DrawState::stunned);

		float xSpeed = speed;
		float ySpeed = speed;
		if (xDirection != 0 && yDirection != 0)
		{
			xSpeed *= movement::DIAGONAL_SPEED;
			ySpeed *= movement::DIAGONAL_SPEED;
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

		//std::cout << "dx: " << targetMovement->dx << "   dy: " << targetMovement->dy << "   xAcceleration: " << targetMovement->xAcceleration << "   yAcceleration: " << targetMovement->yAcceleration << std::endl;
	}
}