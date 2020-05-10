#pragma once

#include <string>
#include <vector>


struct TMXTileSet
{
	int firstGID = 0;
	std::string source = "";
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;
	int spacing = 0;
};

struct TMXTile
{
	int tileSetId = -1;
	int gid = -1;
	bool flippedHorizontally = false;
	bool flippedVertically = false;
	bool flippedDiagonally = false;
};

struct TMXData
{
	std::vector<std::vector<std::vector<TMXTile>>> tileGridLayers;
	std::vector<TMXTileSet> tileSets;
	// height and width in tiles
	int mapWidth = 0;
	int mapHeight = 0;
	int tileWidth = 0;
	int tileHeight = 0;
};

namespace tmxparser
{
	extern const unsigned FLIPPED_HORIZONTALLY_FLAG;
	extern const unsigned FLIPPED_VERTICALLY_FLAG;
	extern const unsigned FLIPPED_DIAGONALLY_FLAG;

	TMXData parseTileMap(const std::string& fileName);
}