#include <algorithm>
#include "game.h"
#include "graphics.h"
#include "systemevents.h"
#include "tilemap.h"

#include "entitymanager.h"
//#include "eventmanager.h"

#include "spritesystem.h"
#include "positionupdatesystem.h"
#include "playersystem.h"

#include <iostream>

const int Game::RESOLUTION_WIDTH = 640;
const int Game::RESOLUTION_HEIGHT = 480;
const int Game::MIN_FRAMERATE = 60;
const float Game::SPRITE_SCALE = 2.0;

// TEMPORARY DELETE LATER
// do need to load tilesheets before use
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
	Input input(eventManager);
	EntityManager entityManager;

	systemEvents::initSystemEvents(*this, eventManager);

	int maxFrameTimeMS = 1000 / MIN_FRAMERATE;
	int deltaTimeMS, currentTimeMS;
	int lastTimeMS = SDL_GetTicks();

	graphics.offsetView(100, 100);
	std::cout << "X: " << graphics.getView().getX() << "   Y: " << graphics.getView().getY() << std::endl;
	TileMap tm(graphics, "content/maps/sample_fantasy.tmx");
	tileMap = &tm;
	graphics.loadTilesheet(filePath, 16, 16, 0);

	entityManager.createEntity(componentManager, 300.f, 200.f, EntityManager::PLAYER);
	entityManager.createEntity(componentManager, 150.f, 150.f, EntityManager::DUMMY);

	SpriteSystem spriteSystem(componentManager, eventManager, graphics);
	PositionUpdateSystem posUpdateSystem(componentManager, eventManager, graphics.getView());
	PlayerSystem playerSystem(componentManager, eventManager);

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

	// TEMP CAMERA
	Rectangle view = graphics.getView();
	auto playerList = componentManager.getComponentList<PlayerComponent>();
	PositionComponent* playerPos = componentManager.getComponent<PositionComponent>(playerList.at(0)->entityId);
	graphics.setView((int)playerPos->x - view.getW() / 2, (int)playerPos->y - view.getH() / 2);

	tileMap->draw(graphics);
	eventManager.fireEvent<DrawEvent>(DrawEvent(&graphics));

	graphics.display();
}

void Game::update(int deltaTime)
{
	eventManager.fireEvent<UpdateEvent>(UpdateEvent(deltaTime));
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