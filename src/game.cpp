#include <algorithm>
#include <time.h>
#include "game.h"
#include "systemevents.h"
#include "tilemap.h"
#include "config.h"

#include <iostream>

// TEMPORARY DELETE LATER
// do need to load tilesheets before use
const std::string filePath = "content/tilesheets/1bit.png";

Game::Game() :
	sdlInit(),
	graphics(config::RESOLUTION_WIDTH, config::RESOLUTION_HEIGHT, 0, "Action Game", config::SPRITE_SCALE),
	tileMap(graphics),
	ecs(eventManager, tileMap),
	gameView(config::RESOLUTION_WIDTH, config::RESOLUTION_HEIGHT, &ecs),
	input(eventManager)
{
	std::srand((unsigned)time(nullptr));
	gameLoop();
}

void Game::gameLoop()
{
	SDL_Event event;

	systemEvents::initSystemEvents(*this, eventManager);

	int maxFrameTimeMS = 1000 / config::MIN_FRAMERATE;
	int deltaTimeMS, currentTimeMS;
	int lastTimeMS = SDL_GetTicks();

	std::unordered_map<int, Rectangle<float>> tileCollisions;
	std::vector<int> solidTileIds = { 33, 68, 137, 138, 417, 326, 227, 387, 386, 385, 354, 369, 418, 401, 453, 359, 556, 297, 
		38, 241, 293, 292, 488, 424, 358, 523, 524, 525, 491, 492, 493, 166, 451, 481, 450, 449, 450, 452, 71, 422, 271, 102 };
	for (int id : solidTileIds)
	{
		tileCollisions.emplace(id, Rectangle<float>(0.f, 0.f, 16.f, 16.f));
	}

	tileMap.initFromTMX("content/maps/sample_fantasy.tmx", tileCollisions);

	graphics.loadTilesheet(filePath, 16, 16, 0);
	graphics.loadTilesheet("content/tilesheets/link.png", 16, 16, 1);
	graphics.loadTilesheet("content/tilesheets/attack.png", 16, 16, 1);
	graphics.loadTilesheet("content/tilesheets/enemies.png", 16, 16, 1);

	gameView.setBounds(Rectangle<int>(0, 0, (int)(tileMap.getWidth()), (int)(tileMap.getHeight())));

	EntityManager& em = ecs.getEntityManager();
	em.createEntity(300.f, 200.f, ECS::DUMMY);
	em.createEntity(332.f, 364.f, ECS::DUMMY);
	em.createEntity(364.f, 364.f, ECS::DUMMY);
	em.createEntity(396.f, 364.f, ECS::DUMMY);
	em.createEntity(428.f, 364.f, ECS::DUMMY);
	em.createEntity(460.f, 364.f, ECS::DUMMY);
	em.createEntity(492.f, 200.f, ECS::DUMMY);

	em.destroyEntity(3);
	em.createEntity(200.f, 200.f, ECS::PLAYER);

	em.createEntity(300.f, 300.f, ECS::ENEMY);
	em.createEntity(350.f, 300.f, ECS::ENEMY);
	em.createEntity(400.f, 300.f, ECS::ENEMY);
	em.createEntity(450.f, 300.f, ECS::ENEMY);
	em.createEntity(500.f, 300.f, ECS::ENEMY);
	
	em.createEntity(200.f, 350.f, ECS::DUMMY);

	/*for (int i = 0; i < 200; i++)
	{
		em.createEntity(-300.f, -600.f, ECS::DUMMY);
	}
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 300.f, ECS::PLAYER);
	em.createEntity(400.f, 300.f, ECS::PLAYER);
	em.createEntity(500.f, 300.f, ECS::PLAYER);
	em.createEntity(600.f, 300.f, ECS::PLAYER);
	em.createEntity(300.f, 400.f, ECS::PLAYER);
	em.createEntity(300.f, 500.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);
	em.createEntity(300.f, 600.f, ECS::PLAYER);*/

	while (gameRunning)
	{
		handleEvents(input, event);

		// Used to cap game speed at min framerate. Slowdown occurs if framerate is below min_framerate
		currentTimeMS = SDL_GetTicks();
		deltaTimeMS = currentTimeMS - lastTimeMS;
		update(std::min(deltaTimeMS, maxFrameTimeMS));

		if (deltaTimeMS > 16)
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
	graphics.drawForeground(gameView.getView());

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
	input.update();
}

void Game::stopGame()
{
	gameRunning = false;
}