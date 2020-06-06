#pragma once

#include "rectangle.h"
#include "system.h"

// forward declare
class Graphics;

class SpriteSystem : public System
{
public:
	SpriteSystem(ComponentManager& compManager);
	void drawSprites(Graphics& graphics, const Rectangle<float>& view);
};