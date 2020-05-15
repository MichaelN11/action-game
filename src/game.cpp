#include <algorithm>
#include "game.h"
#include "graphics.h"
#include "globalsystem.h"
#include "tilemap.h"

#include "eventmanager.h"

#include <iostream>

const int Game::RESOLUTION_WIDTH = 640;
const int Game::RESOLUTION_HEIGHT = 480;
const int Game::MIN_FRAMERATE = 60;
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
	EventManager eventManager;
	SDL_Event event;
	Input input(eventManager);

	globalSystem::initSystemEvents(*this, eventManager);

	int maxFrameTimeMS = 1000 / MIN_FRAMERATE;
	int deltaTimeMS, currentTimeMS;
	int lastTimeMS = SDL_GetTicks();

	graphics.offsetView(100, 100);
	std::cout << "X: " << graphics.getView().getX() << "   Y: " << graphics.getView().getY() << std::endl;
	TileMap tm(graphics, "content/maps/sample_fantasy.tmx");
	tileMap = &tm;
	graphics.loadTilesheet(filePath, 16, 16, 0);

	eventManager.registerListener<DrawEvent>([this](DrawEvent& dEvent)
		{
			if (dEvent.graphics)
				tileMap->draw(*(dEvent.graphics));
		});

	while (gameRunning)
	{
		handleEvents(input, event);

		// Used to cap game speed at min framerate. Slowdown occurs if framerate is below min_framerate
		currentTimeMS = SDL_GetTicks();
		deltaTimeMS = currentTimeMS - lastTimeMS;
		update(std::min(deltaTimeMS, maxFrameTimeMS));

		//std::cout << "DeltaTime: " << deltaTimeMS << ",  Max Frame Time: " << maxFrameTimeMS << std::endl;

		lastTimeMS = currentTimeMS;

		draw(graphics, eventManager);
	}
}

void Game::draw(Graphics& graphics, EventManager& eventManager)
{
	graphics.clear();

	DrawEvent drawEvent(&graphics);
	eventManager.fireEvent<DrawEvent>(drawEvent);

	//tileMap->draw(graphics);

	graphics.display();
}

void Game::update(int timeElapsed)
{

}

void Game::handleEvents(Input& input, SDL_Event& event)
{
	if (SDL_PollEvent(&event))
	{
		input.handleSDLEvent(event);
	}
}

void Game::stopGame()
{
	gameRunning = false;
}