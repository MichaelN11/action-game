#pragma once

#include <vector>
#include "rectangle.h"

// forward declare
struct SDL_Texture;

class TileSheet
{
public:
	TileSheet(SDL_Texture* texture, int width, int height, int tileWidth, int tileHeight, int spacing);
	SDL_Texture* getTexture();

	// Get rectangle for tile in sheet with id of tileNum, if not found return empty rectangle
	Rectangle getTileRect(size_t tileNum) const;

	TileSheet();
private:
	SDL_Texture* texture;
	std::vector<Rectangle> tileRects;
};