#include <algorithm>

#include "tilemap.h"
#include "graphics.h"
#include "tmxparser.h"
#include "rectangle.h"

TileMap::TileMap(Graphics &graphics, const std::string& tmxFileName)
{
	TMXData data = tmxparser::parseTileMap(tmxFileName);
	for (std::vector<std::vector<TMXTile>> tmxLayer : data.tileGridLayers)
	{
		std::vector<std::vector<Tile>> layer;
		
		for (std::vector<TMXTile> tmxRow : tmxLayer)
		{
			std::vector<Tile> row;

			for (TMXTile tmxTile : tmxRow)
			{
				Tile tile;
				tile.flippedDiagonally = tmxTile.flippedDiagonally;
				tile.flippedHorizontally = tmxTile.flippedHorizontally;
				tile.flippedVertically = tmxTile.flippedVertically;
				tile.id = tmxTile.gid;
				tile.tileSheetId = tmxTile.tileSetId;
				row.push_back(tile);
			}

			layer.push_back(row);
		}

		tileGridLayers.push_back(layer);
	}

	for (TMXTileSet tileSet : data.tileSets)
	{
		graphics.loadTilesheet(tileSet.source, tileSet.tileWidth, tileSet.tileHeight, tileSet.spacing);
		tileSheetPaths.push_back(tileSet.source);
	}

	mapWidth = data.mapWidth;
	mapHeight = data.mapHeight;
	tileWidth = data.tileWidth;
	tileHeight = data.tileHeight;
}

void TileMap::drawToBackground(Graphics& graphics)
{
	int scaledTileWidth = (int)(tileWidth * graphics.getScale());
	int scaledTileHeight = (int)(tileHeight * graphics.getScale());

	graphics.createBackgroundTexture(mapWidth * scaledTileWidth, mapHeight * scaledTileHeight);

	for (std::vector<std::vector<Tile>> layer : tileGridLayers)
	{
		for (size_t rowNum = 0; rowNum < layer.size(); rowNum++)
		{
			for (size_t colNum = 0; colNum < layer.at(0).size(); colNum++)
			{
				Tile tile = layer.at(rowNum).at(colNum);
				if (tile.id >= 0 && tile.tileSheetId >= 0)
				{
					Rectangle<int> destRect(colNum * scaledTileWidth,
						rowNum * scaledTileHeight,
						scaledTileWidth,
						scaledTileHeight);

					std::string filePath = tileSheetPaths.at(tile.tileSheetId);
					// scaled is false because it's already scaled here
					graphics.drawToBackgroundTexture(filePath, tile.id, destRect, tile.flippedDiagonally, tile.flippedHorizontally, tile.flippedVertically, false);
				}
			}
		}
	}
}

int TileMap::getWidth() const
{
	return mapWidth * tileWidth;
}

int TileMap::getHeight() const
{
	return mapHeight * tileHeight;
}