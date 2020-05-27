#include "ecs/collisionsystem.h"
#include "ecs/componentmanager.h"
#include "eventmanager.h"
#include "tilemap.h"

#include <algorithm>

CollisionSystem::CollisionSystem(ComponentManager& compManager, EventManager& eventManager, const TileMap& tileMap) :
	compManager(compManager),
	eventManager(eventManager),
	tileMap(tileMap)
{
	eventManager.registerListener<MoveEvent>([this](MoveEvent& mEvent)
	{
		checkCollisions(mEvent.entityId);
	});
}

void CollisionSystem::checkCollisions(int entityId)
{
	auto collision = compManager.getComponent<CollisionComponent>(entityId);
	if (collision)
	{
		auto position = compManager.getComponent<PositionComponent>(entityId);
		if (position)
		{
			if (collision->solid)
			{
				checkTileCollisions(collision, position);
			}

			checkEntityCollisions(collision, position);
		}
	}
}

void CollisionSystem::checkEntityCollisions(CollisionComponent* collisionComp, PositionComponent* positionComp)
{
	// first collision check this update
	if (collisionList.size() == 0)
	{
		auto newCollisionList = compManager.getComponentList<CollisionComponent>();
		for (auto sharedPtr : newCollisionList)
		{
			collisionList.push_back(sharedPtr.get());
		}
	}

	Rectangle<float> boundingBox = collisionComp->boundingBox;
	boundingBox.shift(positionComp->x, positionComp->y);

	float xMoveStored = 0;
	float yMoveStored = 0;

	auto it = collisionList.begin();
	while (it != collisionList.end())
	{
		CollisionComponent* other = *it;
		if (other->entityId != collisionComp->entityId)
		{
			auto entity = compManager.getEntityComponents(other->entityId);
			if (entity->getComponent<InactiveComponent>())
			{
				it = collisionList.erase(it);
			}
			else
			{
				PositionComponent* otherPos = entity->getComponent<PositionComponent>();

				Rectangle<float> otherBox = other->boundingBox;
				otherBox.shift(otherPos->x, otherPos->y);

				if (boundingBox.isColliding(otherBox))
				{
					if (collisionComp->solid && other->solid)
					{
						float xMove, yMove;
						boundingBox.getCollisionDirection(otherBox, xMove, yMove);
						if (xMove == 0 || yMove == 0)
						{
							positionComp->x += xMove;
							positionComp->y += yMove;
							boundingBox.shift(xMove, yMove);
							xMoveStored = 0;
							yMoveStored = 0;
						}
						else
						{
							// allow for sliding along "walls" made up of multiple entities while still checking for diagonal collision
							xMoveStored = xMove;
							yMoveStored = yMove;
						}
					}

					eventManager.fireEvent<CollisionEvent>(CollisionEvent(collisionComp->entityId, other->entityId));
				}

				++it;
			}
		}
		else
		{
			++it;
		}
	}
	
	if (xMoveStored != 0 || yMoveStored != 0)
	{
		positionComp->x += xMoveStored;
		positionComp->y += yMoveStored;
	}
}

void CollisionSystem::checkTileCollisions(CollisionComponent* collisionComp, PositionComponent* positionComp)
{
	const std::vector<std::vector<Rectangle<float>>>& tileCollisionGrid = tileMap.getCollisionGrid();
	if (collisionComp && positionComp && tileCollisionGrid.size() > 0)
	{
		Rectangle<float> boundingBox = collisionComp->boundingBox;
		boundingBox.shift(positionComp->x, positionComp->y);

		int startRow = std::max((int)boundingBox.getY() / tileMap.getScaledTileHeight(), 0);
		int endRow = std::min((int)boundingBox.getY2() / tileMap.getScaledTileHeight() + 2, tileMap.getHeightInTiles());
		int startCol = std::max((int)boundingBox.getX() / tileMap.getScaledTileWidth(), 0);
		int endCol = std::min((int)boundingBox.getX() / tileMap.getScaledTileWidth() + 2, tileMap.getWidthInTiles());

		float xMoveStored = 0;
		float yMoveStored = 0;
		for (int row = startRow; row < endRow; ++row)
		{
			for (int col = startCol; col < endCol; ++col)
			{
				Rectangle<float> tileCollisionBox = tileCollisionGrid.at(row).at(col);
				if (tileCollisionBox.getW() > 0)
				{
					tileCollisionBox.shift((float)tileMap.getScaledTileWidth() * col, (float)tileMap.getScaledTileHeight() * row);
					if (boundingBox.isColliding(tileCollisionBox))
					{
						float xMove, yMove;
						boundingBox.getCollisionDirection(tileCollisionBox, xMove, yMove);
						if (xMove == 0 || yMove == 0)
						{
							positionComp->x += xMove;
							positionComp->y += yMove;
							boundingBox.shift(xMove, yMove);
							xMoveStored = 0;
							yMoveStored = 0;
						}
						else
						{
							// allow for sliding along walls made up of multiple tiles while still checking for diagonal collision
							xMoveStored = xMove;
							yMoveStored = yMove;
						}
					}
				}
			}
		}
		positionComp->x += xMoveStored;
		positionComp->y += yMoveStored;
	}
}

void CollisionSystem::afterUpdate()
{
	collisionList.clear();
}