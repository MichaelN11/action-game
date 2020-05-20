#include "SDL_image.h"
#include "graphics.h"

#include <cmath>

Graphics::Graphics()
{}

void Graphics::init(int width, int height, std::uint32_t windowFlags, std::string title, float spriteScale)
{
	this->spriteScale = spriteScale;
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

void Graphics::drawImage(const std::string& filePath, Rectangle<int> destinationRect, bool scaled)
{
	Texture* texture = getTexture(filePath);
	if (texture)
	{
		if (scaled)
			scaleRect(destinationRect);

		SDL_RenderCopy(renderer, texture->getSDLTexture(), nullptr, &getSDLRect(destinationRect));
	}
}

void Graphics::drawImage(const std::string& filePath, Rectangle<int> sourceRect, Rectangle<int> destinationRect, bool scaled)
{
	Texture* texture = getTexture(filePath);
	if (texture)
	{
		if (scaled)
			scaleRect(destinationRect);

		SDL_RenderCopy(renderer, texture->getSDLTexture(), &getSDLRect(sourceRect), &getSDLRect(destinationRect));
	}
}

void Graphics::drawImage(const std::string& filePath, int tileNum, Rectangle<int> destinationRect, bool scaled)
{
	Texture* texture = getTexture(filePath);
	if (texture)
	{
		if (scaled)
			scaleRect(destinationRect);

		Rectangle<int> sourceRect = texture->getTileRect(tileNum);

		SDL_RenderCopy(renderer, texture->getSDLTexture(), &getSDLRect(sourceRect), &getSDLRect(destinationRect));
	}
}

void Graphics::drawImage(const std::string& filePath, int tileNum, Rectangle<int> destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled)
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

void Graphics::createBackgroundTexture(int width, int height)
{
	background = std::make_unique<Texture>(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height));
	bgWidth = width;
	bgHeight = height;
}

void Graphics::drawToBackgroundTexture(const std::string& filePath, int tileNum, Rectangle<int> destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled)
{
	if (background)
	{
		SDL_SetRenderTarget(renderer, background->getSDLTexture());

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

		// reset render target
		SDL_SetRenderTarget(renderer, nullptr);
	}
}

void Graphics::drawBackground(const Rectangle<float> view)
{
	int sourceX = (view.getX() > 0) ? (int)std::round(view.getX()) : 0;
	int sourceY = (view.getY() > 0) ? (int)std::round(view.getY()) : 0;

	int rectWidth;
	if (view.getX() < 0)
	{
		if (view.getX2() > bgWidth)
			rectWidth = bgWidth;
		else if (view.getX2() > 0)
			rectWidth = (int)(view.getX2() + 0.5f);
		else
			rectWidth = 0;
	}
	else if (view.getX2() > bgWidth)
	{
		if (view.getX() < bgWidth)
			rectWidth = bgWidth - (int)(view.getX() + 0.5f);
		else
			rectWidth = 0;
	}
	else
	{
		rectWidth = (int)(view.getW() + 0.5f);
	}
	int rectHeight;
	if (view.getY() < 0)
	{
		if (view.getY2() > bgHeight)
			rectHeight = bgHeight;
		else if (view.getY2() > 0)
			rectHeight = (int)(view.getY2() + 0.5f);
		else
			rectHeight = 0;
	}
	else if (view.getY2() > bgHeight)
	{
		if (view.getY() < bgHeight)
			rectHeight = bgHeight - (int)(view.getY() + 0.5f);
		else
			rectHeight = 0;
	}
	else
	{
		rectHeight = (int)(view.getH()+ 0.5f);
	}
	int destX = (view.getX() > 0) ? 0 : (int)(view.getX() + 0.5f) * -1;
	int destY = (view.getY() > 0) ? 0 : (int)(view.getY() + 0.5f) * -1;
	Rectangle<int> srcRect(sourceX, sourceY, rectWidth, rectHeight);
	Rectangle<int> destRect(destX, destY, rectWidth, rectHeight);

	SDL_RenderCopy(renderer, background->getSDLTexture(), &getSDLRect(srcRect), &getSDLRect(destRect));
}

float Graphics::getScale() const
{
	return spriteScale;
}

SDL_Rect Graphics::getSDLRect(Rectangle<int> rect)
{
	return SDL_Rect({ rect.getX(), rect.getY(), rect.getW(), rect.getH() });
}

void Graphics::scaleRect(Rectangle<int>& rect)
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