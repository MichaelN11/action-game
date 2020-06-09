#include "ecs/positionupdatesystem.h"
#include "eventmanager.h"
#include "ecs/componentmanager.h"
#include "rectangle.h"

const int PositionUpdateSystem::MAX_ACTIVE_DISTANCE = 100;

PositionUpdateSystem::PositionUpdateSystem(ComponentManager& compManager) :
	System(compManager),
	view(view)
{
}

// For every entity with a position, check if entity is active, if so, check if entity can move and move it, then check if entity is within the active bounds. if not, make it inactive.
void PositionUpdateSystem::positionUpdate(int deltaTime, const Rectangle<float>& view, EventManager& eventManager)
{
	Rectangle<float> activeBounds = Rectangle<float>(view.getX() - MAX_ACTIVE_DISTANCE, view.getY() - MAX_ACTIVE_DISTANCE,
		view.getW() + MAX_ACTIVE_DISTANCE * 2, view.getH() + MAX_ACTIVE_DISTANCE * 2);

	auto positionList = compManager.getComponentList<PositionComponent>();

	for (auto position : positionList)
	{
		auto entity = compManager.getEntityComponents(position->entityId);
		if (!(entity->getComponent<InactiveComponent>()))
		{
			MovementComponent* movement = entity->getComponent<MovementComponent>();
			if (movement)
			{
				// apply half acceleration before movement, half acceleration after
				applyHalfAcceleration(movement, deltaTime);
				// move entities
				if (movement->dx != 0 || movement->dy != 0)
				{
					position->x += movement->dx * deltaTime;
					position->y += movement->dy * deltaTime;
					eventManager.fireEvent(MoveEvent(position->entityId, movement->dx, movement->dy));
				}
				applyHalfAcceleration(movement, deltaTime);
			}

			// if entity not in active bounds and is not a player, make it inactive
			if (!(activeBounds.contains(position->x, position->y)) &&
				!(entity->getComponent<PlayerComponent>()))
			{
				compManager.addComponent<InactiveComponent>(InactiveComponent(position->entityId));
			}
		}
		else
		{
			// if it's inactive and in the bounds, make it active
			if (activeBounds.contains(position->x, position->y))
			{
				compManager.removeComponent<InactiveComponent>(position->entityId);
			}
		}
	}
}

void PositionUpdateSystem::applyHalfAcceleration(MovementComponent* movement, int deltaTime)
{
	if (movement->xAcceleration != 0)
	{
		float preAccelerate = movement->dx;
		movement->dx += movement->xAcceleration * deltaTime * 0.5f;
		// movement sign change
		if (movement->dx * preAccelerate < 0)
		{
			movement->dx = 0;
			movement->xAcceleration = 0;
		}
	}
	if (movement->yAcceleration != 0)
	{
		float preAccelerate = movement->dy;
		movement->dy += movement->yAcceleration * deltaTime * 0.5f;
		// movement sign change
		if (movement->dy * preAccelerate < 0)
		{
			movement->dy = 0;
			movement->yAcceleration = 0;
		}
	}
}