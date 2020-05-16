#pragma once

/*
	globalSystem
	Handles events that can occur anywhere in the game
*/

class Game;
class EventManager;

namespace systemEvents
{
	void initSystemEvents(Game& game, EventManager& eventManager);
	void exitProgram(Game& game);
}