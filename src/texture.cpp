#include "texture.h"
#include "SDL.h"

Texture::Texture(SDL_Texture* texture) :
	sdlTexture(texture)
{}

Texture::Texture(SDL_Texture* texture, int width, int height, int spriteWidth, int spriteHeight, int tileSpacing) :
	sdlTexture(texture)
{
	int widthInTiles = (width + tileSpacing) / (spriteWidth + tileSpacing);
	int heightInTiles = (height + tileSpacing) / (spriteHeight + tileSpacing);
	int totalTiles = widthInTiles * heightInTiles;
	tilesheet.reserve(totalTiles);
	for (int i = 0; i < totalTiles; i++)
	{
		Rectangle tileRect((i % widthInTiles) * (spriteWidth + tileSpacing),
			(i / widthInTiles) * (spriteHeight + tileSpacing),
			spriteWidth,
			spriteHeight);

		tilesheet.push_back(tileRect);
	}
}

Texture::~Texture()
{
	SDL_DestroyTexture(sdlTexture);
}

SDL_Texture* Texture::getSDLTexture()
{
	return sdlTexture;
}

Rectangle Texture::getTileRect(size_t tileNum) const
{
	if (tileNum > 0 && tileNum <= tilesheet.size())
		return tilesheet[tileNum - 1];
	else
		return Rectangle( 0, 0, 0, 0 );
}