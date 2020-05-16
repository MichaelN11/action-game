#include "spritesystem.h"
#include "componentmanager.h"
#include "graphics.h"

void SpriteSystem::drawSprites(ComponentManager& componentManager, Graphics& graphics)
{
	Rectangle view = graphics.getView();
	auto spriteCompList = componentManager.getComponentList<SpriteComponent>();
	for (auto sprite : spriteCompList)
	{
		InactiveComponent* inactive = componentManager.getComponent<InactiveComponent>(sprite->entityId);
		if (!inactive)
		{
			PositionComponent* position = componentManager.getComponent<PositionComponent>(sprite->entityId);
			if (position)
			{
				Rectangle rect(position->x - view.getX(), position->y - view.getY(), sprite->width, sprite->height);
				graphics.drawImage(sprite->filePath, sprite->tileNum, rect, true);
			}
		}
	}
}