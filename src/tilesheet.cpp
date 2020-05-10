#include "tilesheet.h"

TileSheet::TileSheet() :
	texture(nullptr)
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
}

SDL_Texture* TileSheet::getTexture()
{
	return texture;
}

SDL_Rect TileSheet::getTileRect(size_t tileNum) const
{
	if (tileNum > 0 && tileNum <= tileRects.size())
		return tileRects[tileNum - 1];
	else
		return SDL_Rect({ 0, 0, 0, 0 });
}