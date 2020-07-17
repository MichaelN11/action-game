#pragma once

#include "input.h"
#include "eventmanager.h"

Input::Input(EventManager& eventManager) :
	eventManager(&eventManager)
{
	keybindMap.emplace(SDL_SCANCODE_ESCAPE, Keybind::escape);
	keybindMap.emplace(SDL_SCANCODE_UP, Keybind::up);
	keybindMap.emplace(SDL_SCANCODE_DOWN, Keybind::down);
	keybindMap.emplace(SDL_SCANCODE_LEFT, Keybind::left);
	keybindMap.emplace(SDL_SCANCODE_RIGHT, Keybind::right);
	keybindMap.emplace(SDL_SCANCODE_Z, Keybind::attack);
}

void Input::handleSDLEvent(SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_KEYDOWN)
	{
		std::unordered_map<SDL_Scancode, Keybind>::iterator it = keybindMap.find(sdlEvent.key.keysym.scancode);
		if (it != keybindMap.end())
		{
			Keybind key = it->second;
			if (sdlEvent.key.repeat == 0)
			{
				keyDownEvent(key);
			}
		}
	}
	else if (sdlEvent.type == SDL_KEYUP)
	{
		std::unordered_map<SDL_Scancode, Keybind>::iterator it = keybindMap.find(sdlEvent.key.keysym.scancode);
		if (it != keybindMap.end())
		{
			keyUpEvent(it->second);
		}
	}
	else if (sdlEvent.type == SDL_QUIT)
	{
		quitEvent();
	}
}

void Input::keyDownEvent(Keybind key)
{
	heldKeys[key] = true;
	KeyDownEvent kEvent(key, heldKeys);
	eventManager->fireEvent<KeyDownEvent>(kEvent);
}

void Input::keyUpEvent(Keybind key)
{
	heldKeys[key] = false;
	KeyUpEvent kEvent(key, heldKeys);
	eventManager->fireEvent<KeyUpEvent>(kEvent);
}

void Input::keyHeldEvent(Keybind key)
{
	KeyHeldEvent kEvent(key, heldKeys);
	eventManager->fireEvent<KeyHeldEvent>(kEvent);
}

void Input::quitEvent()
{
	QuitEvent qEvent;
	eventManager->fireEvent<QuitEvent>(qEvent);
}

void Input::update()
{
	for (auto pair : heldKeys)
	{
		if (pair.second)
		{
			keyHeldEvent(pair.first);
		}
	}
}