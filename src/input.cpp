#pragma once

#include "input.h"
#include "eventmanager.h"

Input::Input()
{
	keybindMap.emplace(SDL_SCANCODE_ESCAPE, Keybind::escape);
	keybindMap.emplace(SDL_SCANCODE_UP, Keybind::up);
	keybindMap.emplace(SDL_SCANCODE_DOWN, Keybind::down);
	keybindMap.emplace(SDL_SCANCODE_LEFT, Keybind::left);
	keybindMap.emplace(SDL_SCANCODE_RIGHT, Keybind::right);
}

void Input::handleSDLEvent(SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_KEYDOWN)
	{
		std::unordered_map<SDL_Scancode, Keybind>::iterator it = keybindMap.find(sdlEvent.key.keysym.scancode);
		if (it != keybindMap.end())
			keyDownEvent(it->second);
	}
	else if (sdlEvent.type == SDL_KEYUP)
	{
		std::unordered_map<SDL_Scancode, Keybind>::iterator it = keybindMap.find(sdlEvent.key.keysym.scancode);
		if (it != keybindMap.end())
			keyUpEvent(it->second);
	}
	else if (sdlEvent.type == SDL_QUIT)
	{
		quitEvent();
	}
}

void Input::keyDownEvent(Keybind key)
{
	KeyDownEvent kEvent(key);
	EventManager<KeyDownEvent>::fireEvent(kEvent);
}

void Input::keyUpEvent(Keybind key)
{
	KeyUpEvent kEvent(key);
	EventManager<KeyUpEvent>::fireEvent(kEvent);
}

void Input::quitEvent()
{
	QuitEvent qEvent;
	EventManager<QuitEvent>::fireEvent(qEvent);
}