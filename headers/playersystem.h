#pragma once

//forward declare
class ComponentManager;
class EventManager;

class PlayerSystem
{
public:
	PlayerSystem(ComponentManager& compManager, EventManager& eventManager);
private:
	ComponentManager& compManager;
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void stopMoving();
	void setMovespeed(int xDirection, int yDirection);
};