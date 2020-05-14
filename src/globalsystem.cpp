#include "globalsystem.h"
#include "eventmanager.h"
#include "game.h"

void globalSystem::initSystemEvents(Game& game, EventManager& eventManager)
{
	eventManager.registerListener<QuitEvent>([&](QuitEvent& qEvent)
		{
			exitProgram(game);
		});

	eventManager.registerListener<KeyDownEvent>([&](KeyDownEvent& kdEvent)
		{
			if (kdEvent.keyPressed == Keybind::escape)
			{
				exitProgram(game);
			}
		});
}

void globalSystem::exitProgram(Game& game)
{
	game.stopGame();
}