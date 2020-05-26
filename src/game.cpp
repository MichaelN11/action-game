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
	//sdlInit(),
	graphics(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, "Action Game", SPRITE_SCALE),
	tileMap(graphics),
	ecs(eventManager, tileMap),
	gameView(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, &ecs)
{
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

	std::unordered_map<int, Rectangle<float>> tileCollisions;
	std::vector<int> solidTileIds = { 33, 68, 137, 138, 417, 326, 227, 387, 386, 385, 354, 369, 418, 401, 453, 359, 556, 297, 
		38, 241, 293, 292, 488, 424, 358, 523, 524, 525, 491, 492, 493, 166, 451, 481, 450, 449, 450, 452, 71, 422, 271, 102 };
	for (int id : solidTileIds)
	{
		tileCollisions.emplace(id, Rectangle<float>(0.f, 0.f, 32.f, 24.f));
	}

	tileMap.initFromTMX("content/maps/sample_fantasy.tmx", tileCollisions);

	graphics.loadTilesheet(filePath, 16, 16, 0);
	graphics.loadTilesheet("content/tilesheets/link.png", 16, 16, 1);

	tileMap.drawToBackground();

	gameView.setBounds(Rectangle<int>(0, 0, (int)(tileMap.getWidth() * SPRITE_SCALE), (int)(tileMap.getHeight() * SPRITE_SCALE)));

	ecs.createEntity(200.f, 200.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::DUMMY);
	ecs.createEntity(400.f, 300.f, ECS::DUMMY);
	ecs.createEntity(500.f, 300.f, ECS::DUMMY);
	ecs.createEntity(600.f, 300.f, ECS::DUMMY);
	ecs.createEntity(300.f, 400.f, ECS::DUMMY);
	ecs.createEntity(300.f, 500.f, ECS::DUMMY);
	ecs.createEntity(300.f, 600.f, ECS::DUMMY);

	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 300.f, ECS::PLAYER);
	ecs.createEntity(400.f, 300.f, ECS::PLAYER);
	ecs.createEntity(500.f, 300.f, ECS::PLAYER);
	ecs.createEntity(600.f, 300.f, ECS::PLAYER);
	ecs.createEntity(300.f, 400.f, ECS::PLAYER);
	ecs.createEntity(300.f, 500.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);
	ecs.createEntity(300.f, 600.f, ECS::PLAYER);

	eventManager.registerListener<CollisionEvent>([](CollisionEvent e) {});

	while (gameRunning)
	{
		handleEvents(input, event);

		// Used to cap game speed at min framerate. Slowdown occurs if framerate is below min_framerate
		currentTimeMS = SDL_GetTicks();
		deltaTimeMS = currentTimeMS - lastTimeMS;
		update(std::min(deltaTimeMS, maxFrameTimeMS));

		std::cout << "DeltaTime: " << deltaTimeMS << ",  Max Frame Time: " << maxFrameTimeMS << std::endl;

		lastTimeMS = currentTimeMS;

		draw(graphics);
	}
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();

	//std::cout << "x: " << gameView.getView().getX() << ",   y: " << gameView.getView().getY() << std::endl;
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