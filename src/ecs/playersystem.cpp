#include "ecs/playersystem.h"
#include "ecs/componentmanager.h"
#include "eventmanager.h"

//const float PlayerSystem::DIAGONAL_SPEED = 0.7071f;
const float PlayerSystem::DIAGONAL_SPEED = 1.f;

PlayerSystem::PlayerSystem(ComponentManager& compManager, EventManager& eventManager) :
	compManager(compManager)
{
	eventManager.registerListener<KeyDownEvent>([this](KeyDownEvent& kdEvent)
		{
			if (kdEvent.keyPressed == Keybind::up ||
				kdEvent.keyPressed == Keybind::down ||
				kdEvent.keyPressed == Keybind::left ||
				kdEvent.keyPressed == Keybind::right)
			{
				setMovement(kdEvent.heldKeys);
			}
		});
	eventManager.registerListener<KeyUpEvent>([this](KeyUpEvent& kuEvent)
		{
			if (kuEvent.keyReleased == Keybind::up ||
				kuEvent.keyReleased == Keybind::down ||
				kuEvent.keyReleased == Keybind::left ||
				kuEvent.keyReleased == Keybind::right)
			{
				setMovement(kuEvent.heldKeys);
			}
		});
}

void PlayerSystem::setMovement(const std::unordered_map<Keybind, bool>& heldKeys)
{
	bool up = false, down = false, left = false, right = false;
	auto upIt = heldKeys.find(Keybind::up);
	if (upIt != heldKeys.end())
		up = upIt->second;
	auto downIt = heldKeys.find(Keybind::down);
	if (downIt != heldKeys.end())
		down = downIt->second;
	auto leftIt = heldKeys.find(Keybind::left);
	if (leftIt != heldKeys.end())
		left = leftIt->second;
	auto rightIt = heldKeys.find(Keybind::right);
	if (rightIt != heldKeys.end())
		right = rightIt->second;

	if (up && left && !down && !right)
	{
		setMovespeed(-DIAGONAL_SPEED, -DIAGONAL_SPEED);
	}
	else if (up && right && !down && !left)
	{
		setMovespeed(DIAGONAL_SPEED, -DIAGONAL_SPEED);
	}
	else if (down && right && !up && !left)
	{
		setMovespeed(DIAGONAL_SPEED, DIAGONAL_SPEED);
	}
	else if (down && left && !up && !right)
	{
		setMovespeed(-DIAGONAL_SPEED, DIAGONAL_SPEED);
	}
	else if (up && !down)
	{
		setMovespeed(0, -1);
	}
	else if (right && !left)
	{
		setMovespeed(1, 0);
	}
	else if (down && !up)
	{
		setMovespeed(0, 1);
	}
	else if (left && !right)
	{
		setMovespeed(-1, 0);
	}
	else
	{
		setMovespeed(0, 0);
	}
}

void PlayerSystem::setMovespeed(float xDirection, float yDirection)
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