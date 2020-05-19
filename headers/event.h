#pragma once

#include "enums.h"

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

struct QuitEvent : Event
{};

//struct UpdateEvent : Event
//{
//	UpdateEvent(int deltaTime) : deltaTime(deltaTime) {}
//	int deltaTime;
//};

//struct DrawEvent : Event
//{
//	DrawEvent(Graphics* g) : graphics(g) {}
//	Graphics* graphics = nullptr;
//};