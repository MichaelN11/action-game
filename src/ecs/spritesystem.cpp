#include <cmath>

#include "ecs/spritesystem.h"
#include "ecs/componentmanager.h"
#include "graphics.h"
#include "eventmanager.h"

SpriteSystem::SpriteSystem(ComponentManager& compManager) :
	System(compManager)
{
}

void SpriteSystem::drawSprites(Graphics& graphics, const Rectangle<float>& view)
{
	auto spriteCompMap = compManager.getComponentSorted<SpriteComponent>();
	for (auto layerSpritePair : spriteCompMap)
	{
		SpriteComponent* sprite = layerSpritePair.second.get();
		if (sprite->visible)
		{
			auto entity = compManager.getEntityComponents(sprite->entityId);
			InactiveComponent* inactive = entity->getComponent<InactiveComponent>();
			if (!inactive)
			{
				PositionComponent* position = entity->getComponent<PositionComponent>();
				if (position)
				{
					float destX = position->x + sprite->xOffset - view.getX();
					float destY = position->y + sprite->yOffset - view.getY();
					Rectangle<float> rect(destX, destY, (float)sprite->width, (float)sprite->height);
					graphics.drawImage(sprite->filePath, sprite->tileNum, rect, sprite->flipDiagonal, sprite->flipHorizontal, sprite->flipVertical, true);
				}
			}
		}
	}
}