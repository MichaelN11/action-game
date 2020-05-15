#pragma once

#include <string>
#include <vector>

#include "eventmanager.h"

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
	TileMap(Graphics &graphics, const std::string& tmxFileName);
	void draw(Graphics& graphics);
private:
	// height and width in tiles
	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
	std::vector<std::string> tileSheetPaths;
	std::vector<std::vector<std::vector<Tile>>> tileGridLayers;
};

struct Tile
{
	int tileSheetId = -1;
	int id = 0;
	bool flippedHorizontally = false;
	bool flippedVertically = false;
	bool flippedDiagonally = false;
};