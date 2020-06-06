#include "ecs/movement.h"

void movement::standardMove(ComponentManager::EntityComponents* entity, float xMultiplier, float yMultiplier)
{
	MovementComponent* movement = entity->getComponent<MovementComponent>();
	if (movement)
	{
		StateComponent* state = entity->getComponent<StateComponent>();
		if (state)
		{
			bool standing = (state->bufferedDrawState == DrawState::standDown ||
				state->bufferedDrawState == DrawState::standLeft ||
				state->bufferedDrawState == DrawState::standUp ||
				state->bufferedDrawState == DrawState::standRight);

			if (yMultiplier == 0 && xMultiplier == 0)
			{
				switch (state->drawState)
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
					state->setBufferedDrawState(DrawState::walkRight);
					if (standing)
						state->drawState = DrawState::walkRight;
				}
				else if (xMultiplier < 0)
				{
					state->setBufferedDrawState(DrawState::walkLeft);
					if (standing)
						state->drawState = DrawState::walkLeft;
				}
			}
			else if (xMultiplier == 0)
			{
				if (yMultiplier > 0)
				{
					state->setBufferedDrawState(DrawState::walkDown);
					if (standing)
						state->drawState = DrawState::walkDown;
				}
				else if (yMultiplier < 0)
				{
					state->setBufferedDrawState(DrawState::walkUp);
					if (standing)
						state->drawState = DrawState::walkUp;
				}
			}
		}

		movement->dx = movement->moveSpeed * xMultiplier;
		movement->dy = movement->moveSpeed * yMultiplier;
	}
}