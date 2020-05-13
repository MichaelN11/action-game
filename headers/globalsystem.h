#pragma once

/*
	globalSystem
	Handles events that can occur anywhere in the game
*/

class Game;

namespace globalSystem
{
	void initSystemEvents(Game& game);
	void exitProgram(Game& game);
}