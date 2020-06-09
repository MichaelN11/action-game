#pragma once

#include "enums.h"
#include "ecs/componentmanager.h"

class Graphics;

struct Event
{};

struct KeyDownEvent : Event
{
	KeyDownEvent(Keybind k, const std::unordered_map<Keybind, bool>& heldKeys) : keyPressed(k), heldKeys(heldKeys) {}
	Keybind keyPressed;
	const std::unordered_map<Keybind, bool>& heldKeys;
};

struct KeyUpEvent : Event
{
	KeyUpEvent(Keybind k, const std::unordered_map<Keybind, bool>& heldKeys) : keyReleased(k), heldKeys(heldKeys) {}
	Keybind keyReleased;
	const std::unordered_map<Keybind, bool>& heldKeys;
};

struct KeyHeldEvent : Event
{
	KeyHeldEvent(Keybind k, const std::unordered_map<Keybind, bool>& heldKeys) : keyHeld(k), heldKeys(heldKeys)
	{}
	Keybind keyHeld;
	const std::unordered_map<Keybind, bool>& heldKeys;
};

struct QuitEvent : Event
{};

struct MoveEvent : Event
{
	MoveEvent(int entityId, float dx, float dy) : entityId(entityId), dx(dx), dy(dy) {}
	int entityId;
	float dx;
	float dy;
};

struct DamageEvent : Event
{
	DamageEvent(ComponentManager::EntityComponents* source, ComponentManager::EntityComponents* target, float xDirection, float yDirection) : 
		source(source), target(target), xDirection(xDirection), yDirection(yDirection) 
	{}
	ComponentManager::EntityComponents* source;
	ComponentManager::EntityComponents* target;
	float xDirection;
	float yDirection;
};