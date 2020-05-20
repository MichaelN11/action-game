#include <algorithm>
#include "game.h"
#include "systemevents.h"
#include "tilemap.h"

#include <iostream>

const int Game::RESOLUTION_WIDTH = 640;
const int Game::RESOLUTION_HEIGHT = 480;
const int Game::MIN_FRAMERATE = 60;
const float Game::SPRITE_SCALE = 2.0;

// TEMPORARY DELETE LATER
// do need to load tilesheets before use
const std::string filePath = "content/tilesheets/1bit.png";

Game::Game() :
	ecs(eventManager),
	gameView(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, &ecs)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	graphics.init(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, "Action Game", SPRITE_SCALE);
	gameLoop();
}

void Game::gameLoop()
{
	SDL_Event event;
	Input input(eventManager);

	systemEvents::initSystemEvents(*this, eventManager);

	int maxFrameTimeMS = 1000 / MIN_FRAMERATE;
	int deltaTimeMS, currentTimeMS;
	int lastTimeMS = SDL_GetTicks();

	TileMap tm(graphics, "content/maps/sample_fantasy.tmx");
	tileMap = &tm;
	graphics.loadTilesheet(filePath, 16, 16, 0);
	tileMap->drawToBackground(graphics);

	gameView.setBounds(Rectangle<int>(0, 0, (int)(tileMap->getWidth() * SPRITE_SCALE), (int)(tileMap->getHeight() * SPRITE_SCALE)));

	ecs.createEntity(300.f, 200.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::DUMMY);
	ecs.createEntity(400.f, 300.f, ECS::DUMMY);
	ecs.createEntity(500.f, 300.f, ECS::DUMMY);
	ecs.createEntity(600.f, 300.f, ECS::DUMMY);
	ecs.createEntity(300.f, 400.f, ECS::DUMMY);
	ecs.createEntity(300.f, 500.f, ECS::DUMMY);
	ecs.createEntity(300.f, 600.f, ECS::DUMMY);

	while (gameRunning)
	{
		handleEvents(input, event);

		// Used to cap game speed at min framerate. Slowdown occurs if framerate is below min_framerate
		currentTimeMS = SDL_GetTicks();
		deltaTimeMS = currentTimeMS - lastTimeMS;
		update(std::min(deltaTimeMS, maxFrameTimeMS));

		//std::cout << "DeltaTime: " << deltaTimeMS << ",  Max Frame Time: " << maxFrameTimeMS << std::endl;

		lastTimeMS = currentTimeMS;

		draw(graphics);
	}
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();

	std::cout << "x: " << gameView.getView().getX() << ",   y: " << gameView.getView().getY() << std::endl;
	//tileMap->draw(graphics);
	graphics.drawBackground(gameView.getView());
	ecs.draw(graphics, gameView.getView());

	graphics.display();
}

void Game::update(int deltaTime)
{
	gameView.update(deltaTime);
	ecs.update(deltaTime, gameView.getView());
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