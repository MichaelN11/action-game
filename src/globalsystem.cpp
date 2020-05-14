#include "globalsystem.h"
#include "eventmanager.h"
#include "game.h"

void globalSystem::initSystemEvents(Game& game)
{
	EventManager<QuitEvent>::registerListener([&](QuitEvent& qEvent)
		{
			exitProgram(game);
		});

	EventManager<KeyDownEvent>::registerListener([&](KeyDownEvent& kdEvent)
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