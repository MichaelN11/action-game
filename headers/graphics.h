#pragma once

#include <string>
#include <unordered_map>
#include "SDL.h"

/*
	Graphics class
	Responsible for handling the SDL window and renderer, and drawing images to the window
*/

// forward declare
class TileSheet;
class Rectangle;

class Graphics
{
public:
	Graphics(int width, int height, std::uint32_t windowFlags, std::string title, float spriteScale);
	~Graphics();

	// First checks the textureMap to see if the filePath texture has already been loaded,
	// if not, loads it into the map, freeing the surface used. Returns the texture from the map.
	SDL_Texture* loadImage(const std::string& filePath);
	// Same but stores width and height of the surface
	SDL_Texture* loadImage(const std::string& filePath, int& width, int& height);

	// Draws the image from the source texture or filePath, drawing the source rectangle into the destination rectangle
	void drawImage(SDL_Texture* source, Rectangle sourceRect, Rectangle destinationRect, bool scaled);
	void drawImage(const std::string &filePath, Rectangle sourceRect, Rectangle destinationRect, bool scaled);
	// Tile sheet must already be loaded
	void drawImage(const std::string& tileSheetFilePath, int tileNum, Rectangle destinationRect, bool scaled);
	void drawImage(const std::string& tileSheetFilePath, int tileNum, Rectangle destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled);

	// Displays the renderer to the window
	void display();

	// Clears the renderer
	void clear();

	// TileSheets hold the texture and a vector of rectangles containing each tile
	// Loads tilesheet and returns it
	TileSheet* loadTileSheet(const std::string& filePath, int tileWidth, int tileHeight, int spacing);
	// Tries to get an already loaded tilesheet and return null if not found
	TileSheet* getTileSheet(const std::string& filePath);

	float getScale();

	Graphics() = delete;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
private:
	SDL_Rect getSDLRect(Rectangle rect);
	void scaleRect(Rectangle& rect);
	// convert from TMX style 3 flip flags to SDL style rotation/flipping
	void convertRotation(float& angle, int& flip, bool flipDiagonal, bool flipHorizontal, bool flipVertical);

	float spriteScale;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unordered_map<std::string, SDL_Texture*> textureMap;
	std::unordered_map<std::string, TileSheet> tileSheetMap;
};