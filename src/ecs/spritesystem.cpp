#include <cmath>

#include "ecs/spritesystem.h"
#include "ecs/componentmanager.h"
#include "graphics.h"
#include "eventmanager.h"
SpriteSystem::SpriteSystem(ComponentManager& compManager) :
	compManager(compManager)
{
}

void SpriteSystem::drawSprites(Graphics& graphics)
{
	Rectangle view = graphics.getView();
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
				Rectangle rect((int)position->x - view.getX(), (int)position->y - view.getY(), sprite->width, sprite->height);
				graphics.drawImage(sprite->filePath, sprite->tileNum, rect, true);
			}
		}
	}
}