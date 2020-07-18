#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "SDL.h"
#include "rectangle.h"
#include "texture.h"

/*
	Graphics class
	Responsible for handling the SDL window and renderer, and drawing images to the window
	Also holds a map of textures with the filenames as keys
*/


class Graphics
{
public:
	Graphics(int width, int height, std::uint32_t windowFlags, std::string title, float spriteScale);
	~Graphics();

	// First checks the textureMap to see if the filePath texture has already been loaded,
	// if not, loads it into the map, freeing the surface used. Returns the texture from the map.
	bool loadImage(const std::string& filePath);
	// Tilesheets hold the texture and a vector of rectangles containing each tile
	// Stored inside the texture
	bool loadTilesheet(const std::string& filePath, int tileWidth, int tileHeight, int tileSpacing);

	// Draws the image from the source texture or filePath, drawing the source rectangle into the destination rectangle
	void drawImage(const std::string& filePath, Rectangle<int> destinationRect, bool scaled);
	void drawImage(const std::string& filePath, Rectangle<int> sourceRect, Rectangle<int> destinationRect, bool scaled);
	// Tile sheet must already be loaded
	void drawImage(const std::string& filePath, int tileNum, Rectangle<int> destinationRect, bool scaled);
	void drawImage(const std::string& filePath, int tileNum, Rectangle<int> destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled);

	// Displays the renderer to the window
	void display();

	// Clears the renderer
	void clear();

	// Destroys textures and removes all references
	void unloadImage(const std::string& filePath);
	void unloadAllImages();

	// Draws to a texture that is then drawn all at once, useful for drawing tilemaps
	void createBackgroundTexture(int width, int height);
	void createForegroundTexture(int width, int height);
	void drawToBackgroundTexture(const std::string& filePath, int tileNum, Rectangle<int> destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled);
	void drawToForegroundTexture(const std::string& filePath, int tileNum, Rectangle<int> destinationRect, bool flipDiagonal, bool flipHorizontal, bool flipVertical, bool scaled);
	void drawBackground(Rectangle<float> view);
	void drawForeground(Rectangle<float> view);
	void drawTextureToScreen(Rectangle<float> view, Texture* texture, int tWidth, int tHeight);
	


	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
private:
	float getScale() const;

	Texture* getTexture(const std::string& filePath);
	SDL_Rect getSDLRect(Rectangle<int> rect);
	void scaleRect(Rectangle<int>& rect);
	// convert from TMX style 3 flip flags to SDL style rotation/flipping
	void convertRotation(float& angle, int& flip, bool flipDiagonal, bool flipHorizontal, bool flipVertical);

	float spriteScale;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unordered_map<std::string, Texture> textureMap;
	std::unique_ptr<Texture> background;
	std::unique_ptr<Texture> foreground;
	int bgWidth = 0, bgHeight = 0;
	int fgWidth = 0, fgHeight = 0;
};
