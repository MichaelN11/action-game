#include "SDL_image.h"
#include "graphics.h"
#include "tilesheet.h"

Graphics::Graphics(int width, int height, std::uint32_t windowFlags, std::string title, float spriteScale) :
	spriteScale(spriteScale),
	view(Rectangle(0, 0, width, height))
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

void Graphics::drawImage(SDL_Texture* source, Rectangle sourceRect, Rectangle destinationRect, bool scaled)
{
	if (scaled)
	{
		scaleRect(destinationRect);
	}
	SDL_RenderCopy(renderer, source, &getSDLRect(sourceRect), &getSDLRect(destinationRect));
}

void Graphics::drawImage(const std::string& filePath, Rectangle sourceRect, Rectangle destinationRect, bool scaled)
{
	SDL_Texture* texture = loadImage(filePath);
	drawImage(texture, sourceRect, destinationRect, scaled);
}

void Graphics::drawImage(const std::string& tileSheetFilePath, int tileNum, Rectangle destinationRect, bool scaled)
{
	TileSheet* tileSheet = getTileSheet(tileSheetFilePath);
	drawImage(tileSheet->getTexture(), tileSheet->getTileRect(tileNum), destinationRect, scaled);
}

void Graphics::drawImage(const std::string& tileSheetFilePath, int tileNum, Rectangle destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled)
{
	TileSheet* tileSheet = getTileSheet(tileSheetFilePath);
	
	float angle;
	int flip;

	convertRotation(angle, flip, flipDiagonal, flipHorizontal, flipVertical);

	if (scaled)
	{
		scaleRect(destinationRect);
	}

	SDL_RenderCopyEx(renderer, tileSheet->getTexture(), &getSDLRect(tileSheet->getTileRect(tileNum)), &getSDLRect(destinationRect), angle, nullptr, (SDL_RendererFlip)flip);
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

void Graphics::offsetView(int xOffset, int yOffset)
{
	view.setX(view.getX() + xOffset);
	view.setY(view.getY() + yOffset);
}

Rectangle Graphics::getView() const
{
	return view;
}

float Graphics::getScale() const
{
	return spriteScale;
}

SDL_Rect Graphics::getSDLRect(Rectangle rect)
{
	return SDL_Rect({ rect.getX(), rect.getY(), rect.getW(), rect.getH() });
}

void Graphics::scaleRect(Rectangle& rect)
{
	rect.setH((int)(rect.getH() * spriteScale));
	rect.setW((int)(rect.getW() * spriteScale));
}

void Graphics::convertRotation(float& angle, int& flip, bool flipDiagonal, bool flipHorizontal, bool flipVertical)
{
	angle = 0;
	flip = SDL_FLIP_NONE;

	if (!flipHorizontal && flipVertical && flipDiagonal)
	{
		angle = 270;
	}
	else if (flipHorizontal && flipVertical && !flipDiagonal)
	{
		angle = 180;
	}
	else if (flipHorizontal && !flipVertical && flipDiagonal)
	{
		angle = 90;
	}
	else if (!flipHorizontal && flipVertical && !flipDiagonal)
	{
		flip = SDL_FLIP_VERTICAL;
	}
	else if (flipHorizontal && flipVertical && flipDiagonal)
	{
		flip = SDL_FLIP_VERTICAL;
		angle = 270;
	}
	else if (flipHorizontal && !flipVertical && !flipDiagonal)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if (!flipHorizontal && !flipVertical && flipDiagonal)
	{
		flip = SDL_FLIP_HORIZONTAL;
		angle = 270;
	}
}
