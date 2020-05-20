#pragma once

#include <vector>
#include "rectangle.h"

/*
	Texture class
	Manages the SDL_Texture, destroys it when destroyed
	Also holds a vector of rectangles representing tiles on a tilesheet
	If texture is not a tilesheet, vector is empty
*/

// forward declare
struct SDL_Texture;

class Texture
{
public:
	Texture(SDL_Texture* texture);
	// Constructs tilesheet
	Texture(SDL_Texture* texture, int width, int height, int tileWidth, int tileHeight, int tileSpacing);
	~Texture();

	SDL_Texture* getSDLTexture();

	// Get rectangle for tile in sheet with id of tileNum, if not found return empty rectangle
	Rectangle<int> getTileRect(size_t tileNum) const;

	Texture() = delete;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
private:
	SDL_Texture* sdlTexture;
	std::vector<Rectangle<int>> tilesheet;
};