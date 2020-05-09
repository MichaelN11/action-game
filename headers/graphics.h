#pragma once

#include <string>
#include <unordered_map>

/*
	Graphics class
	Responsible for handling the SDL window and renderer, and drawing images to the window
*/

// forward declare
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

class Graphics
{
public:
	Graphics(int width, int height, std::uint32_t windowFlags, std::string title, float spriteScale);
	~Graphics();

	// First checks the textureMap to see if the filePath texture has already been loaded,
	// if not, loads it into the map, freeing the surface used. Returns the texture from the map.
	SDL_Texture* loadImage(const std::string& filePath);

	// Draws the image from the source texture or filePath, drawing the source rectangle into the destination rectangle
	void drawImage(SDL_Texture* source, SDL_Rect* sourceRect, SDL_Rect* destinationRect, bool scaled);
	void drawImage(const std::string &filePath, SDL_Rect* sourceRect, SDL_Rect* destinationRect, bool scaled);

	// Displays the renderer to the window
	void display();

	// Clears the renderer
	void clear();

	Graphics() = delete;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
private:
	float spriteScale;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unordered_map<std::string, SDL_Texture*> textureMap;
};