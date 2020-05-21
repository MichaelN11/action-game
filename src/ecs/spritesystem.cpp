#include <cmath>

#include "ecs/spritesystem.h"
#include "ecs/componentmanager.h"
#include "graphics.h"
#include "eventmanager.h"

SpriteSystem::SpriteSystem(ComponentManager& compManager) :
	compManager(compManager)
{
}

void SpriteSystem::drawSprites(Graphics& graphics, const Rectangle<float>& view)
{
	auto spriteCompMap = compManager.getComponentSorted<SpriteComponent>();
	for (auto layerSpritePair : spriteCompMap)
	{
		SpriteComponent* sprite = layerSpritePair.second.get();
		InactiveComponent* inactive = compManager.getComponent<InactiveComponent>(sprite->entityId);
		if (!inactive)
		{
			PositionComponent* position = compManager.getComponent<PositionComponent>(sprite->entityId);
			if (position)
			{
				int destX = (int)std::round(position->x - view.getX());
				int destY = (int)std::round(position->y - view.getY());
				Rectangle<int> rect(destX, destY, sprite->width, sprite->height);
				graphics.drawImage(sprite->filePath, sprite->tileNum, rect, sprite->flipDiagonal, sprite->flipHorizontal, sprite->flipVertical, true);
			}
		}
	}
}