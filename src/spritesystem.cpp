#include "spritesystem.h"
#include "componentmanager.h"
#include "graphics.h"
#include "eventmanager.h"

SpriteSystem::SpriteSystem(ComponentManager& compManager, EventManager& eventManager, Graphics& graphics) :
	compManager(compManager)
{
	eventManager.registerListener<DrawEvent>([&](DrawEvent& dEvent)
		{
			if (dEvent.graphics)
			{
				drawSprites(graphics);
			}
		});
}

void SpriteSystem::drawSprites(Graphics& graphics)
{
	Rectangle view = graphics.getView();
	auto spriteCompList = compManager.getComponentList<SpriteComponent>();
	for (auto sprite : spriteCompList)
	{
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