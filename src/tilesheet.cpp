#include "tilesheet.h"

// REMOVE LATER
#include <iostream>

TileSheet::TileSheet() :
	texture(NULL)
{}

TileSheet::TileSheet(SDL_Texture* texture, int width, int height, int spriteWidth, int spriteHeight, int spacing) :
	texture(texture)
{
	int widthInTiles = (width + spacing) / (spriteWidth + spacing);
	int heightInTiles = (height + spacing) / (spriteHeight + spacing);
	int totalTiles = widthInTiles * heightInTiles;
	tileRects.reserve(totalTiles);
	for (int i = 0; i < totalTiles; i++)
	{
		SDL_Rect tileRect;
		tileRect.x = (i % widthInTiles) * (spriteWidth + spacing);
		tileRect.y = (i / widthInTiles) * (spriteHeight + spacing);
		tileRect.w = spriteWidth;
		tileRect.h = spriteHeight;

		tileRects.push_back(tileRect);
	}

	std::cout << "CALLED TILESHEET CONSTRUCTOR" << std::endl;
}

SDL_Texture* TileSheet::getTexture()
{
	return texture;
}

SDL_Rect TileSheet::getTileRect(int tileNum) const
{
	if (tileNum >= 0 && tileNum < tileRects.size())
		return tileRects[tileNum];
	else
		return SDL_Rect({ 0, 0, 0, 0 });
}