#include "tilesheet.h"
#include "SDL.h"

#include "iostream"

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
		Rectangle tileRect((i % widthInTiles) * (spriteWidth + spacing),
			(i / widthInTiles) * (spriteHeight + spacing),
			spriteWidth,
			spriteHeight);

		tileRects.push_back(tileRect);
	}
}

SDL_Texture* TileSheet::getTexture()
{
	return texture;
}

Rectangle TileSheet::getTileRect(size_t tileNum) const
{
	if (tileNum > 0 && tileNum <= tileRects.size())
		return tileRects[tileNum - 1];
	else
		return Rectangle( 0, 0, 0, 0 );
}