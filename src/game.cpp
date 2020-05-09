#include <algorithm>
#include "game.h"
#include "graphics.h"
#include "SDL.h"

#include <iostream>

const int Game::RESOLUTION_WIDTH = 640;
const int Game::RESOLUTION_HEIGHT = 480;
const int Game::MIN_FRAMERATE = 30;
const float Game::SPRITE_SCALE = 2.0;

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

	SDL_Rect sourceRect = { 0, 16, 16, 16 };
	SDL_Rect destRect = { 100, 100, 16, 16 };
	const std::string filePath = "content/sprites/1bit.png";
	SDL_Texture* texture = graphics.loadImage(filePath);
	graphics.drawImage(texture, &sourceRect, &destRect, true);

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