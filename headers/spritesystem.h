#pragma once

// forward declare
class EventManager;
class ComponentManager;
class Graphics;

class SpriteSystem
{
public:
	SpriteSystem(ComponentManager& compManager, EventManager& eventManager, Graphics& graphics);
	void drawSprites(Graphics& graphics);
private:
	ComponentManager& compManager;
};