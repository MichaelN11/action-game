#pragma once

#include <string>
#include <unordered_map>
#include "SDL.h"
#include "rectangle.h"

/*
	Graphics class
	Responsible for handling the SDL window and renderer, and drawing images to the window
	Also holds a map of textures with the filenames as keys
*/

// forward declare
class Texture;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void init(int width, int height, std::uint32_t windowFlags, std::string title, float spriteScale);

	// First checks the textureMap to see if the filePath texture has already been loaded,
	// if not, loads it into the map, freeing the surface used. Returns the texture from the map.
	bool loadImage(const std::string& filePath);
	// Tilesheets hold the texture and a vector of rectangles containing each tile
	// Stored inside the texture
	bool loadTilesheet(const std::string& filePath, int tileWidth, int tileHeight, int tileSpacing);

	// Draws the image from the source texture or filePath, drawing the source rectangle into the destination rectangle
	void drawImage(const std::string& filePath, Rectangle destinationRect, bool scaled);
	void drawImage(const std::string& filePath, Rectangle sourceRect, Rectangle destinationRect, bool scaled);
	// Tile sheet must already be loaded
	void drawImage(const std::string& filePath, int tileNum, Rectangle destinationRect, bool scaled);
	void drawImage(const std::string& filePath, int tileNum, Rectangle destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled);

	// Displays the renderer to the window
	void display();

	// Clears the renderer
	void clear();

	// Destroys textures and removes all references
	void unloadImage(const std::string& filePath);
	void unloadAllImages();
	
	void offsetView(int xOffset, int yOffset);
	void setView(int x, int y);
	Rectangle getView() const;
	float getScale() const;

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
private:
	Texture* getTexture(const std::string& filePath);
	SDL_Rect getSDLRect(Rectangle rect);
	void scaleRect(Rectangle& rect);
	// convert from TMX style 3 flip flags to SDL style rotation/flipping
	void convertRotation(float& angle, int& flip, bool flipDiagonal, bool flipHorizontal, bool flipVertical);

	float spriteScale;
	Rectangle view;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unordered_map<std::string, Texture> textureMap;
};
