#include "ecs/positionupdatesystem.h"
#include "eventmanager.h"
#include "ecs/componentmanager.h"
#include "rectangle.h"

const int PositionUpdateSystem::MAX_ACTIVE_DISTANCE = 100;

PositionUpdateSystem::PositionUpdateSystem(ComponentManager& compManager) :
	compManager(compManager),
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
				if (movement->dx != 0 || movement->dy != 0)
				{
					position->x += movement->dx * deltaTime;
					position->y += movement->dy * deltaTime;
					eventManager.fireEvent(MoveEvent(position->entityId, movement->dx, movement->dy));
				}
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