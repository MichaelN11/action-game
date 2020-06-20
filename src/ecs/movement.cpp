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

				float moveSpeed = movement->moveSpeed;

				if (xMultiplier != 0 && yMultiplier != 0)
				{
					moveSpeed *= DIAGONAL_SPEED;
				}

				movement->dx = moveSpeed * xMultiplier;
				movement->dy = moveSpeed * yMultiplier;
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

void movement::knockback(ComponentManager::EntityComponents* entity, float angle, float speed, float deceleration)
{
	MovementComponent* targetMovement = entity->getComponent<MovementComponent>();
	if (targetMovement)
	{
		StateComponent* targetState = entity->getComponent<StateComponent>();
		targetState->activityState = ActivityState::stunned;
		targetState->stunTimer = (int)(speed / deceleration);
		targetState->previousDrawState = targetState->getBufferedDrawState();
		targetState->setDrawState(DrawState::stunned);

		//std::cout << "Angle: " << angle << std::endl;

		float xAngle = cos(angle);
		float yAngle = sin(angle);

		targetMovement->dx = 0;
		targetMovement->dy = 0;

		targetMovement->dx = speed * xAngle;
		// multiply by negative 1 because programming y is opposite sign as trig y
		targetMovement->dy = (speed * yAngle) * -1;

		// deceleration in opposite direction to movement
		targetMovement->xAcceleration = deceleration * xAngle * -1;
		targetMovement->yAcceleration = deceleration * yAngle;


		//std::cout << "dx: " << targetMovement->dx << "   dy: " << targetMovement->dy << "   xAcceleration: " << targetMovement->xAcceleration << "   yAcceleration: " << targetMovement->yAcceleration << std::endl;
	}
}