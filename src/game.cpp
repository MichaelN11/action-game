#include <algorithm>
#include "game.h"
#include "graphics.h"
#include "tilemap.h"

#include "eventmanager.h"

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
	Input input;

	int maxFrameTimeMS = 1000 / MIN_FRAMERATE;
	int deltaTimeMS, currentTimeMS;
	int lastTimeMS = SDL_GetTicks();

	graphics.offsetView(100, 100);
	std::cout << "X: " << graphics.getView().getX() << "   Y: " << graphics.getView().getY() << std::endl;
	TileMap tm(graphics, "content/maps/sample_fantasy.tmx");
	tileMap = &tm;
	graphics.loadTilesheet(filePath, 16, 16, 0);

	//class MyKeyListener : public EventListener<KeyDownEvent>
	//{
	//public:
	//	Graphics* parent;
	//	MyKeyListener(Graphics& parent) : parent(&parent) {}
	//	void onEvent(KeyDownEvent& kEvent)
	//	{
	//		//std::cout << "got here!" << std::endl;
	//		Rectangle view = parent->getView();
	//		switch (kEvent.keyPressed)
	//		{
	//		case Keybind::up :
	//			//if (view.getY() > 0)
	//				parent->offsetView(0, -3);
	//			break;
	//		case Keybind::down :
	//			//if (view.getY() < 768 - view.getH())
	//				parent->offsetView(0, 3);
	//			break;
	//		case Keybind::left :
	//			//if (view.getX() > 0)
	//				parent->offsetView(-3, 0);
	//			break;
	//		case Keybind::right :
	//			//if (view.getX() < 1536 - view.getW())
	//				parent->offsetView(3, 0);
	//			break;
	//		}
	//	}
	//};
	//EventManager::registerListener<KeyDownEvent, MyKeyListener, Graphics>(graphics);

	bool running = true;
	while (running)
	{
		handleEvents(input, event);

		// Used to cap game speed at min framerate. Slowdown occurs if framerate is below min_framerate
		currentTimeMS = SDL_GetTicks();
		deltaTimeMS = currentTimeMS - lastTimeMS;
		update(std::min(deltaTimeMS, maxFrameTimeMS));

		lastTimeMS = currentTimeMS;

		std::cout << "DT: " << deltaTimeMS << "    MFT: " << maxFrameTimeMS << std::endl;

		draw(graphics);
	}
}

void Game::draw(Graphics& graphics)
{
	graphics.clear();

	DrawEvent drawEvent(&graphics);
	EventManager::fireEvent<DrawEvent>(drawEvent);

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