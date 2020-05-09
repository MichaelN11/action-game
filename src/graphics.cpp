#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"

Graphics::Graphics(int width, int height, std::uint32_t windowFlags, std::string title, float spriteScale) :
	spriteScale(spriteScale)
{
	SDL_CreateWindowAndRenderer(width, height, windowFlags, &window, &renderer);
	SDL_SetWindowTitle(window, title.c_str());
}

Graphics::~Graphics()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

SDL_Texture* Graphics::loadImage(const std::string& filePath)
{
	if (textureMap.count(filePath) == 0)
	{
		SDL_Surface* surface = IMG_Load(filePath.c_str());
		textureMap[filePath] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	return textureMap[filePath];
}

void Graphics::drawImage(SDL_Texture* source, SDL_Rect* sourceRect, SDL_Rect* destinationRect, bool scaled)
{
	if (scaled)
	{
		destinationRect->h = (int)(destinationRect->h * spriteScale);
		destinationRect->w = (int)(destinationRect->w * spriteScale);
	}
	SDL_RenderCopy(renderer, source, sourceRect, destinationRect);
}

void Graphics::drawImage(const std::string& filePath, SDL_Rect* sourceRect, SDL_Rect* destinationRect, bool scaled)
{
	SDL_Texture* texture = loadImage(filePath);
	drawImage(texture, sourceRect, destinationRect, scaled);
}

void Graphics::display()
{
	SDL_RenderPresent(renderer);
}

void Graphics::clear()
{
	SDL_RenderClear(renderer);
}

