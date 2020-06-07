#include "ecs/movement.h"

void movement::standardMove(ComponentManager::EntityComponents* entity, float xMultiplier, float yMultiplier)
{
	MovementComponent* movement = entity->getComponent<MovementComponent>();
	if (movement)
	{
		StateComponent* state = entity->getComponent<StateComponent>();
		if (state)
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
		}

		movement->dx = movement->moveSpeed * xMultiplier;
		movement->dy = movement->moveSpeed * yMultiplier;
	}
}