#pragma once

/*
	Game class
	The main game loop is here
*/

#include "input.h"
#include "eventmanager.h"
#include "ecs/ecs.h"
#include "graphics.h"
#include "gameview.h"
#include "tilemap.h"

// forward declaration
//class EventManager;
class Graphics;
union SDL_Event;

struct SDLInit
{
	SDLInit()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
	}
};

class Game
{
public:
	Game();
	void stopGame();
private:
	// SDLInit needs to be first
	SDLInit sdlInit;
	bool gameRunning = true;
	Graphics graphics;
	EventManager eventManager;
	ECS ecs;
	GameView gameView;

	void gameLoop();
	void draw(Graphics &graphics);
	void update(int timeElapsed);
	void handleEvents(Input& input, SDL_Event& event);

	//temporary
	TileMap tileMap;
};