#pragma once

/*
	Game class
	The main game loop is here
*/

#include "input.h"
#include "eventmanager.h"
#include "componentmanager.h"

// forward declaration
//class EventManager;
class Graphics;
union SDL_Event;

//temporary
class TileMap;

class Game
{
public:
	Game();
	void stopGame();
private:
	bool gameRunning = true;
	EventManager eventManager;
	ComponentManager componentManager;

	void gameLoop();
	void draw(Graphics &graphics);
	void update(int timeElapsed);
	void handleEvents(Input& input, SDL_Event& event);

	static const int RESOLUTION_WIDTH;
	static const int RESOLUTION_HEIGHT;
	static const int MIN_FRAMERATE;
	static const float SPRITE_SCALE;

	//temporary
	TileMap* tileMap;
};