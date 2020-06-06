#include "ecs/playersystem.h"
#include "ecs/componentmanager.h"
#include "eventmanager.h"
#include "ecs/movement.h"

const float PlayerSystem::DIAGONAL_SPEED = 0.7071f;
//const float PlayerSystem::DIAGONAL_SPEED = 1.f;

PlayerSystem::PlayerSystem(ComponentManager& compManager, EventManager& eventManager) :
	System(compManager)
{
	eventManager.registerListener<KeyDownEvent>([this](KeyDownEvent& kdEvent)
		{
			if (kdEvent.keyPressed == Keybind::up ||
				kdEvent.keyPressed == Keybind::down ||
				kdEvent.keyPressed == Keybind::left ||
				kdEvent.keyPressed == Keybind::right)
			{
				parseMovement(kdEvent.heldKeys);
			}
		});
	eventManager.registerListener<KeyUpEvent>([this](KeyUpEvent& kuEvent)
		{
			if (kuEvent.keyReleased == Keybind::up ||
				kuEvent.keyReleased == Keybind::down ||
				kuEvent.keyReleased == Keybind::left ||
				kuEvent.keyReleased == Keybind::right)
			{
				parseMovement(kuEvent.heldKeys);
			}
		});
}

void PlayerSystem::parseMovement(const std::unordered_map<Keybind, bool>& heldKeys)
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
		updateMovement(-DIAGONAL_SPEED, -DIAGONAL_SPEED);
	}
	else if (up && right && !down && !left)
	{
		updateMovement(DIAGONAL_SPEED, -DIAGONAL_SPEED);
	}
	else if (down && right && !up && !left)
	{
		updateMovement(DIAGONAL_SPEED, DIAGONAL_SPEED);
	}
	else if (down && left && !up && !right)
	{
		updateMovement(-DIAGONAL_SPEED, DIAGONAL_SPEED);
	}
	else if (up && !down)
	{
		updateMovement(0, -1);
	}
	else if (right && !left)
	{
		updateMovement(1, 0);
	}
	else if (down && !up)
	{
		updateMovement(0, 1);
	}
	else if (left && !right)
	{
		updateMovement(-1, 0);
	}
	else
	{
		updateMovement(0, 0);
	}
}

void PlayerSystem::updateMovement(float xDirection, float yDirection)
{
	auto playerList = compManager.getComponentList<PlayerComponent>();
	for (auto player : playerList)
	{
		auto entity = compManager.getEntityComponents(player->entityId);
		if (entity && !(entity->getComponent<InactiveComponent>()))
		{
			movement::standardMove(entity, xDirection, yDirection);
		}
	}
}