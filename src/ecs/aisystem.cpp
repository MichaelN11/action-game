#include "ecs/aisystem.h"
#include "ecs/componentmanager.h"
#include "ecs/movement.h"

AISystem::AISystem(ComponentManager& compManager) : System(compManager)
{}

void AISystem::update(int deltaTime)
{
	auto aiList = compManager.getComponentList<AIComponent>();
	for (auto ai : aiList)
	{
		if (ai->behavior != Behavior::none)
		{
			auto entity = compManager.getEntityComponents(ai->entityId);
			if (!(entity->getComponent<InactiveComponent>()))
			{
				ai->timer += deltaTime;
				if (ai->behavior == Behavior::wander)
				{
					MovementComponent* movement = entity->getComponent<MovementComponent>();
					if (movement)
					{
						if (movement->dx == 0.f && movement->dy == 0.f)
						{
							if (ai->timer >= 2000)
							{
								ai->timer = 0;

								float dx = 0, dy = 0;
								int direction = std::rand() % 4;
								switch (direction)
								{
								case 1:
									dx = 1;
									break;
								case 2:
									dx = -1;
									break;
								case 3:
									dy = 1;
									break;
								case 4:
									dy = -1;
									break;
								}

								movement::standardMove(entity, dx, dy);
							}
						}
						else
						{
							if (ai->timer >= 2000)
							{
								ai->timer = 0;

								movement::standardMove(entity, 0, 0);
							}
						}
					}
				}
			}
		}
	}
}