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

class Game
{
public:
	Game();
	void stopGame();
private:
	bool gameRunning = true;
	Graphics graphics;
	EventManager eventManager;
	ECS ecs;
	GameView gameView;

	void gameLoop();
	void draw(Graphics &graphics);
	void update(int timeElapsed);
	void handleEvents(Input& input, SDL_Event& event);

	static const int RESOLUTION_WIDTH;
	static const int RESOLUTION_HEIGHT;
	static const int MIN_FRAMERATE;
	static const float SPRITE_SCALE;

	//struct SDLInit
	//{
	//	SDLInit()
	//	{
	//		SDL_Init(SDL_INIT_EVERYTHING);
	//	}
	//};

	//SDLInit sdlInit;

	//temporary
	TileMap tileMap;
};