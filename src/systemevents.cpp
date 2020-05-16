#include "systemevents.h"
#include "eventmanager.h"
#include "game.h"

void systemEvents::initSystemEvents(Game& game, EventManager& eventManager)
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

void systemEvents::exitProgram(Game& game)
{
	game.stopGame();
}