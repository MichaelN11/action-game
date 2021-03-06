#pragma once

#include <unordered_map>

#include "SDL.h"

#include "enums.h"

class EventManager;

class Input
{
public:
	Input(EventManager& eventManager);
	void handleSDLEvent(SDL_Event& sdlEvent);
	void keyDownEvent(Keybind key);
	void keyUpEvent(Keybind key);
	void keyHeldEvent(Keybind key);
	void quitEvent();

	void update();
private:
	std::unordered_map<SDL_Scancode, Keybind> keybindMap;
	std::unordered_map<Keybind, bool> heldKeys;
	EventManager* eventManager;
};