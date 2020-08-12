#include "ecs/collisionsystem.h"
#include "eventmanager.h"
#include "tilemap.h"

#include <algorithm>


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DELETE LATER
//#include <iostream>

CollisionSystem::CollisionSystem(ComponentManager& compManager, EventManager& eventManager, const TileMap& tileMap) :
	System(compManager),
	eventManager(eventManager),
	tileMap(tileMap)
{
	eventManager.registerListener<MoveEvent>([this](MoveEvent& mEvent)
	{
		checkCollisions(mEvent.entityId);
	});

	eventManager.registerListener<CollisionCheckEvent>([this](CollisionCheckEvent& mEvent)
	{
		checkCollisions(mEvent.entityId);
	});
}

void CollisionSystem::checkCollisions(int entityId)
{
	ComponentManager::EntityComponents* entity = compManager.getEntityComponents(entityId);
	auto collision = entity->getComponent<CollisionComponent>();
	if (collision)
	{
		auto position = entity->getComponent<PositionComponent>();
		if (position)
		{
			// flag is set every time collisions are checked, if entity collides with something solid it's set to true
			collision->collidedLastCheck = false;

			if (collision->collideWithEntities)
			{
				checkEntityCollisions(collision, position, entity);
			}

			if (collision->solid)
			{
				checkTileCollisions(collision, position);
			}

			//for (int checkId : checkList)
			//{
			//	//eventManager.fireEvent<CollisionCheckEvent>(CollisionCheckEvent(checkId));
			//}
			//checkList.clear();
		}
	}
}

void CollisionSystem::checkEntityCollisions(CollisionComponent* collisionComp, PositionComponent* positionComp, ComponentManager::EntityComponents* entityComponents)
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
			auto otherEntity = compManager.getEntityComponents(other->entityId);
			if (otherEntity->getComponent<InactiveComponent>() || !(other->collideWithEntities))
			{
				it = collisionList.erase(it);
			}
			else
			{
				PositionComponent* otherPos = otherEntity->getComponent<PositionComponent>();

				Rectangle<float> otherBox = other->boundingBox;
				otherBox.shift(otherPos->x, otherPos->y);

				if (boundingBox.isColliding(otherBox))
				{
					// collision events
					if (collisionComp->interactable && other->interactable)
					{
						// damage events
						// negative angle to knockback other away from entity
						if (entityComponents->getComponent<DamageComponent>())
						{
							float collisionAngle = otherBox.getCollisionAngle(boundingBox);
							eventManager.fireEvent<DamageEvent>(DamageEvent(entityComponents, otherEntity, collisionAngle));
						}
						// positive xmove and ymove to knockback entity away from other
						if (otherEntity->getComponent<DamageComponent>())
						{
							float collisionAngle = boundingBox.getCollisionAngle(otherBox);
							eventManager.fireEvent<DamageEvent>(DamageEvent(otherEntity, entityComponents, collisionAngle));
						}
					}

					// solid entity collisions
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

						// check if other entity is still colliding after collision resolution
						//checkList.push_back(other->entityId);

						collisionComp->collidedLastCheck = true;
					}
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

		int startRow = std::max((int)boundingBox.getY() / tileMap.getTileHeight(), 0);
		int endRow = std::min((int)boundingBox.getY2() / tileMap.getTileHeight() + 2, tileMap.getHeightInTiles());
		int startCol = std::max((int)boundingBox.getX() / tileMap.getTileWidth(), 0);
		int endCol = std::min((int)boundingBox.getX() / tileMap.getTileWidth() + 2, tileMap.getWidthInTiles());

		float xMoveStored = 0;
		float yMoveStored = 0;
		for (int row = startRow; row < endRow; ++row)
		{
			for (int col = startCol; col < endCol; ++col)
			{
				Rectangle<float> tileCollisionBox = tileCollisionGrid.at(row).at(col);
				if (tileCollisionBox.getW() > 0)
				{
					tileCollisionBox.shift((float)tileMap.getTileWidth() * col, (float)tileMap.getTileHeight() * row);
					if (boundingBox.isColliding(tileCollisionBox))
					{
						float xMove, yMove;
						boundingBox.getCollisionDirection(tileCollisionBox, xMove, yMove);
						if (xMove == 0 || yMove == 0)
						{
							//std::cout << "player    x: " << positionComp->x << "   y: " << positionComp->y << std::endl;

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

						collisionComp->collidedLastCheck = true;
					}
				}
			}
		}
		if (xMoveStored != 0 || yMoveStored != 0)
		{
			//std::cout << "xMoveStored: " << xMoveStored << "   yMoveStored: " << yMoveStored << std::endl;
			positionComp->x += xMoveStored;
			positionComp->y += yMoveStored;
		}

		// check for collision with edge of map
		float xMapOffset = 0;
		float yMapOffset = 0;
		if (boundingBox.getX() < 0.f)
		{
			xMapOffset = boundingBox.getX() * -1.f;
		}
		else if (boundingBox.getX2() > tileMap.getWidth())
		{
			xMapOffset = tileMap.getWidth() - boundingBox.getX2();
		}
		if (boundingBox.getY() < 0.f)
		{
			yMapOffset = boundingBox.getY() * -1.f;
		}
		else if (boundingBox.getY2() > tileMap.getHeight())
		{
			yMapOffset = tileMap.getHeight() - boundingBox.getY2();
		}

		if (xMapOffset != 0.f || yMapOffset != 0.f)
		{
			collisionComp->collidedLastCheck = true;
			positionComp->x += xMapOffset;
			positionComp->y += yMapOffset;
		}
	}
}

void CollisionSystem::afterUpdate()
{
	collisionList.clear();
}

bool CollisionSystem::isCollision(const TileMap& tileMap, Rectangle<float> boundingBox, float x, float y)
{
	const std::vector<std::vector<Rectangle<float>>>& tileCollisionGrid = tileMap.getCollisionGrid();
	boundingBox.shift(x, y);

	int startRow = std::max((int)boundingBox.getY() / tileMap.getTileHeight(), 0);
	int endRow = std::min((int)boundingBox.getY2() / tileMap.getTileHeight() + 2, tileMap.getHeightInTiles());
	int startCol = std::max((int)boundingBox.getX() / tileMap.getTileWidth(), 0);
	int endCol = std::min((int)boundingBox.getX() / tileMap.getTileWidth() + 2, tileMap.getWidthInTiles());

	for (int row = startRow; row < endRow; ++row)
	{
		for (int col = startCol; col < endCol; ++col)
		{
			Rectangle<float> tileCollisionBox = tileCollisionGrid.at(row).at(col);
			if (tileCollisionBox.getW() > 0)
			{
				tileCollisionBox.shift((float)tileMap.getTileWidth() * col, (float)tileMap.getTileHeight() * row);
				if (boundingBox.isColliding(tileCollisionBox))
				{
					return true;
				}
			}
		}
	}

	return false;
}