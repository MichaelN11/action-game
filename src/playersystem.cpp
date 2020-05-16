#include "playersystem.h"
#include "componentmanager.h"
#include "eventmanager.h"

PlayerSystem::PlayerSystem(ComponentManager& compManager, EventManager& eventManager) :
	compManager(compManager)
{
	eventManager.registerListener<KeyDownEvent>([this](KeyDownEvent& kdEvent)
		{
			switch (kdEvent.keyPressed)
			{
				case Keybind::up:
					moveUp();
					break;
				case Keybind::down:
					moveDown();
					break;
				case Keybind::left:
					moveLeft();
					break;
				case Keybind::right:
					moveRight();
					break;
			}
		});
	eventManager.registerListener<KeyUpEvent>([this](KeyUpEvent& kuEvent)
		{
			switch (kuEvent.keyReleased)
			{
			case Keybind::up:
			case Keybind::down:
			case Keybind::left:
			case Keybind::right:
				stopMoving();
				break;
			}
		});
}

void PlayerSystem::stopMoving()
{
	setMovespeed(0, 0);
}

void PlayerSystem::moveUp()
{
	setMovespeed(0, -1);
}
void PlayerSystem::moveDown()
{
	setMovespeed(0, 1);
}
void PlayerSystem::moveLeft()
{
	setMovespeed(-1, 0);
}
void PlayerSystem::moveRight()
{
	setMovespeed(1, 0);
}

void PlayerSystem::setMovespeed(int xDirection, int yDirection)
{
	auto playerList = compManager.getComponentList<PlayerComponent>();
	for (auto player : playerList)
	{
		if (!(compManager.getComponent<InactiveComponent>(player->entityId)))
		{
			MovementComponent* movement = compManager.getComponent<MovementComponent>(player->entityId);
			if (movement)
			{
				movement->dx = movement->moveSpeed * xDirection;
				movement->dy = movement->moveSpeed * yDirection;
			}
		}
	}
}