#include "ecs/collisionsystem.h"
#include "ecs/componentmanager.h"
#include "eventmanager.h"
#include "tilemap.h"

#include <algorithm>

CollisionSystem::CollisionSystem(ComponentManager& compManager, EventManager& eventManager, const TileMap& tileMap) :
	compManager(compManager),
	tileMap(tileMap)
{
	eventManager.registerListener<MoveEvent>([this](MoveEvent& mEvent)
	{
		if (this->compManager.getComponent<SolidComponent>(mEvent.entityId))
		{
			checkSolidCollisions(mEvent.entityId);
		}
	});
}

void CollisionSystem::checkSolidCollisions(int entityId)
{
	auto position = compManager.getComponent<PositionComponent>(entityId);
	auto boundingBoxComponent = compManager.getComponent<BoundingBoxComponent>(entityId);
	const std::vector<std::vector<Rectangle<float>>>& tileCollisionGrid = tileMap.getCollisionGrid();
	if (boundingBoxComponent && position && tileCollisionGrid.size() > 0)
	{
		Rectangle<float> boundingBox = boundingBoxComponent->boundingBox;
		boundingBox.shift(position->x, position->y);

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
							position->x += xMove;
							position->y += yMove;
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
		position->x += xMoveStored;
		position->y += yMoveStored;
	}
}