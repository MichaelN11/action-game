#pragma once

#include <unordered_map>

#include "SDL.h"

#include "enums.h"

class Input
{
public:
	Input();
	void handleSDLEvent(SDL_Event& sdlEvent);
	void keyDownEvent(Keybind key);
	void keyUpEvent(Keybind key);
	void quitEvent();
private:
	std::unordered_map<SDL_Scancode, Keybind> keybindMap;
};