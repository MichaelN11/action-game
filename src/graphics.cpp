#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "tilesheet.h"

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

SDL_Texture* Graphics::loadImage(const std::string& filePath, int &width, int &height)
{
	if (textureMap.count(filePath) == 0)
	{
		SDL_Surface* surface = IMG_Load(filePath.c_str());
		width = surface->w;
		height = surface->h;
		textureMap[filePath] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	return textureMap[filePath];
}

SDL_Texture* Graphics::loadImage(const std::string& filePath)
{
	int temp;
	return loadImage(filePath, temp, temp);
}

void Graphics::drawImage(SDL_Texture* source, SDL_Rect* sourceRect, SDL_Rect* destinationRect, bool scaled)
{
	if (destinationRect && scaled)
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

void Graphics::drawImage(const std::string& tileSheetFilePath, int tileNum, SDL_Rect* destinationRect, bool scaled)
{
	TileSheet* tileSheet = getTileSheet(tileSheetFilePath);
	drawImage(tileSheet->getTexture(), &(tileSheet->getTileRect(tileNum)), destinationRect, scaled);
}

void Graphics::display()
{
	SDL_RenderPresent(renderer);
}

void Graphics::clear()
{
	SDL_RenderClear(renderer);
}

TileSheet* Graphics::loadTileSheet(const std::string& filePath, int tileWidth, int tileHeight, int spacing)
{
	if (tileSheetMap.count(filePath) == 0)
	{
		int width, height;
		SDL_Texture* texture = loadImage(filePath, width, height);
		tileSheetMap.emplace(filePath, TileSheet(texture, width, height, tileWidth, tileHeight, spacing));
	}
	return &tileSheetMap[filePath];
}

TileSheet* Graphics::getTileSheet(const std::string& filePath)
{
	return &tileSheetMap[filePath];
}

float Graphics::getScale()
{
	return spriteScale;
}

