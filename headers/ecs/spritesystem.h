#pragma once

// forward declare
class EventManager;
class ComponentManager;
class Graphics;

class SpriteSystem
{
public:
	SpriteSystem(ComponentManager& compManager);
	void drawSprites(Graphics& graphics);
private:
	ComponentManager& compManager;
};