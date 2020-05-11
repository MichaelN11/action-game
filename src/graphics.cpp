#include "SDL_image.h"
#include "graphics.h"
#include "texture.h"

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

bool Graphics::loadImage(const std::string& filePath)
{
	if (textureMap.count(filePath) == 0)
	{
		SDL_Surface* surface = IMG_Load(filePath.c_str());
		if (surface)
		{
			SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
			textureMap.emplace(std::piecewise_construct,
				std::forward_as_tuple(filePath),
				std::forward_as_tuple(sdlTexture));
			SDL_FreeSurface(surface);
		}
	}
	return (textureMap.count(filePath) > 0);
}

bool Graphics::loadTilesheet(const std::string& filePath, int tileWidth, int tileHeight, int tileSpacing)
{
	if (textureMap.count(filePath) == 0)
	{
		SDL_Surface* surface = IMG_Load(filePath.c_str());
		if (surface)
		{
			SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
			textureMap.emplace(std::piecewise_construct,
				std::forward_as_tuple(filePath),
				std::forward_as_tuple(sdlTexture, surface->w, surface->h, tileWidth, tileHeight, tileSpacing));
			SDL_FreeSurface(surface);
		}
	}
	return (textureMap.count(filePath) > 0);
}

void Graphics::drawImage(const std::string& filePath, Rectangle destinationRect, bool scaled)
{
	Texture* texture = getTexture(filePath);
	if (texture)
	{
		if (scaled)
			scaleRect(destinationRect);

		SDL_RenderCopy(renderer, texture->getSDLTexture(), nullptr, &getSDLRect(destinationRect));
	}
}

void Graphics::drawImage(const std::string& filePath, Rectangle sourceRect, Rectangle destinationRect, bool scaled)
{
	Texture* texture = getTexture(filePath);
	if (texture)
	{
		if (scaled)
			scaleRect(destinationRect);

		SDL_RenderCopy(renderer, texture->getSDLTexture(), &getSDLRect(sourceRect), &getSDLRect(destinationRect));
	}
}

void Graphics::drawImage(const std::string& filePath, int tileNum, Rectangle destinationRect, bool scaled)
{
	Texture* texture = getTexture(filePath);
	if (texture)
	{
		if (scaled)
			scaleRect(destinationRect);

		SDL_RenderCopy(renderer, texture->getSDLTexture(), &getSDLRect(texture->getTileRect(tileNum)), &getSDLRect(destinationRect));
	}
}

void Graphics::drawImage(const std::string& filePath, int tileNum, Rectangle destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled)
{
	Texture* texture = getTexture(filePath);

	if (texture)
	{
		float angle = 0;
		int flip = SDL_FLIP_NONE;
		convertRotation(angle, flip, flipDiagonal, flipHorizontal, flipVertical);

		if (scaled)
			scaleRect(destinationRect);

		SDL_RenderCopyEx(renderer, texture->getSDLTexture(), &getSDLRect(texture->getTileRect(tileNum)), &getSDLRect(destinationRect), angle, nullptr, (SDL_RendererFlip)flip);
	}
}

void Graphics::display()
{
	SDL_RenderPresent(renderer);
}

void Graphics::clear()
{
	SDL_RenderClear(renderer);
}

void Graphics::unloadImage(const std::string& filePath)
{
	textureMap.erase(filePath);
}

void Graphics::unloadAllImages()
{
	textureMap.clear();
}

Texture* Graphics::getTexture(const std::string& filePath)
{
	std::unordered_map<std::string, Texture>::iterator it = textureMap.find(filePath);
	if (it != textureMap.end())
		return &(it->second);
	else
		return nullptr;
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