#include "ecs/aisystem.h"
#include "ecs/componentmanager.h"
#include "ecs/movement.h"
#include "tilemap.h"
#include "ecs/collisionsystem.h"

// REMOVE LATER %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//#include <iostream>

AISystem::AISystem(ComponentManager& compManager, const TileMap& tileMap) : System(compManager), tileMap(tileMap)
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
						CollisionComponent* collision = entity->getComponent<CollisionComponent>();

						if (movement->dx == 0.f && movement->dy == 0.f)
						{
							if (ai->timer >= 2000)
							{
								ai->timer = 0;
								std::vector<int> directionList = { 1, 2, 3, 4 };
								float dx = 0, dy = 0;

								// pick a random direction until one is found with a clear path, or until all directions tried
								do {
									dx = 0;
									dy = 0;
									int index = std::rand() % directionList.size();
									switch (directionList.at(index))
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
									directionList.erase(directionList.begin() + index);
								} while (directionList.size() > 0 && pathBlocked(entity, collision, dx, dy));

								movement::standardMove(entity, dx, dy);
							}
						}
						else
						{
							if (ai->timer >= 2000
								|| (collision && collision->collidedLastCheck))
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

// either there is a tile collision in the direction being moved in, or the entity will end up off the map
bool AISystem::pathBlocked(ComponentManager::EntityComponents* entity, CollisionComponent* collision, float dx, float dy)
{
	PositionComponent* position = entity->getComponent<PositionComponent>();

	if (collision && position)
	{
		float xNext = position->x + ((dx > 0) ? 1.f : ((dx < 0) ? -1.f : 0));
		float yNext = position->y + ((dy > 0) ? 1.f : ((dy < 0) ? -1.f : 0));

		return
			(
			CollisionSystem::isCollision(tileMap, collision->boundingBox, xNext, yNext)
			|| xNext < 0
			|| yNext < 0
			|| collision->boundingBox.getW() + xNext > tileMap.getWidth()
			|| collision->boundingBox.getH() + yNext > tileMap.getHeight()
			);
	}

	return false;
}