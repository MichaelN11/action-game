#pragma once

#include "enums.h"

class Graphics;

struct Event
{};

struct DrawEvent : Event
{
	DrawEvent(Graphics* g) : graphics(g) {}
	Graphics* graphics = nullptr;
};

struct KeyDownEvent : Event
{
	KeyDownEvent(Keybind k) : keyPressed(k) {}
	Keybind keyPressed;
};

struct KeyUpEvent : Event
{
	KeyUpEvent(Keybind k) : keyReleased(k) {}
	Keybind keyReleased;
};

struct QuitEvent : Event
{};

struct TestEvent : Event
{};