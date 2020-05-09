#pragma once

// forward declaration
class Graphics;
union SDL_Event;

class Game
{
public:
	Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void update(int timeElapsed);
	void handleEvents(SDL_Event& event);

	static const int RESOLUTION_WIDTH;
	static const int RESOLUTION_HEIGHT;
	static const int MIN_FRAMERATE;
	static const float SPRITE_SCALE;
};