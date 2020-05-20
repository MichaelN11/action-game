#pragma once

#include "rectangle.h"

// forward declare
class EventManager;
class ComponentManager;
class Graphics;

class SpriteSystem
{
public:
	SpriteSystem(ComponentManager& compManager);
	void drawSprites(Graphics& graphics, const Rectangle<float>& view);
private:
	ComponentManager& compManager;
};