#pragma once

#include <string>
#include <vector>

#include "eventmanager.h"
#include "rectangle.h"

/*
	TileMap class
	Responsible for constructing and drawing the tilemap of a level
*/

//forward declare
class Graphics;

struct Tile;

class TileMap
{
public:
	TileMap(Graphics &graphics);
	void initFromTMX(const std::string& tmxFileName, const std::unordered_map<int, Rectangle<float>>& tileCollisions);
	// Draws the entire tilemap into the background texture
	void drawToBackground();
	int getWidth() const;
	int getHeight() const;
	int getScaledTileWidth() const;
	int getScaledTileHeight() const;
	int getHeightInTiles() const;
	int getWidthInTiles() const;
	const std::vector<std::vector<Rectangle<float>>>& getCollisionGrid() const;
private:
	Graphics& graphics;
	// height and width in tiles
	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
	std::vector<std::string> tileSheetPaths;
	std::vector<std::vector<std::vector<Tile>>> tileGridLayers;
	std::vector<std::vector<Rectangle<float>>> collisionGrid;
};

struct Tile
{
	int tileSheetId = -1;
	int id = 0;
	bool flippedHorizontally = false;
	bool flippedVertically = false;
	bool flippedDiagonally = false;
};