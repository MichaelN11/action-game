#pragma once

#include <vector>
#include "SDL.h"

class TileSheet
{
public:
	TileSheet(SDL_Texture* texture, int width, int height, int tileWidth, int tileHeight, int spacing);
	SDL_Texture* getTexture();
	SDL_Rect getTileRect(size_t tileNum) const;

	TileSheet();
private:
	SDL_Texture* texture;
	std::vector<SDL_Rect> tileRects;
};