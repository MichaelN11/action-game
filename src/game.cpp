#include <algorithm>
#include "game.h"
#include "graphics.h"

#include "tilesheet.h"

#include <iostream>

const int Game::RESOLUTION_WIDTH = 640;
const int Game::RESOLUTION_HEIGHT = 480;
const int Game::MIN_FRAMERATE = 30;
const float Game::SPRITE_SCALE = 2.0;

// TEMPORARY DELETE LATER
const std::string filePath = "content/tilesheets/1bit.png";

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	gameLoop();
}

void Game::gameLoop()
{
	Graphics graphics(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, "Action Game", SPRITE_SCALE);
	SDL_Event event;

	int maxFrameTimeMS = 1000 / MIN_FRAMERATE;
	int deltaTimeMS, currentTimeMS;
	int lastTimeMS = SDL_GetTicks();

	graphics.loadTileSheet(filePath, 16, 16, 0);

	bool running = true;
	while (running)
	{
		handleEvents(event);

		// Used to cap game speed at min framerate. Slowdown occurs if framerate is below min_framerate
		currentTimeMS = SDL_GetTicks();
		deltaTimeMS = currentTimeMS - lastTimeMS;
		update(std::min(deltaTimeMS, maxFrameTimeMS));
		lastTimeMS = currentTimeMS;

		draw(graphics);
	}
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();

	SDL_Rect destRect = { 100, 100, 16, 16 };
	TileSheet* tileSheet = graphics.getTileSheet(filePath);
	graphics.drawImage(tileSheet->getTexture(), &(tileSheet->getTileRect(1054)), &destRect, true);

	graphics.display();
}

void Game::update(int timeElapsed)
{

}

void Game::handleEvents(SDL_Event& event)
{
	if (SDL_PollEvent(&event))
	{

	}
}