#include <algorithm>

#include "tilemap.h"
#include "graphics.h"
#include "tmxparser.h"
#include "rectangle.h"

TileMap::TileMap(Graphics& graphics) : graphics(graphics)
{}

void TileMap::initFromTMX(const std::string& tmxFileName, const std::unordered_map<int, Rectangle<float>>& tileCollisions)
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

	// creating collision grid
	int collisionLayerNum = 0;
	for (std::vector<Tile> tileRow : tileGridLayers.at(collisionLayerNum))
	{
		std::vector<Rectangle<float>> collisionRow;
		for (Tile tile : tileRow)
		{
			auto it = tileCollisions.find(tile.id);
			if (it != tileCollisions.end())
			{
				Rectangle<float> rect = it->second;
				collisionRow.push_back(rect);
			}
			else
			{
				collisionRow.push_back(Rectangle<float>());
			}
		}
		collisionGrid.push_back(collisionRow);
	}

	mapWidth = data.mapWidth;
	mapHeight = data.mapHeight;
	tileWidth = data.tileWidth;
	tileHeight = data.tileHeight;

	drawToBackground();
	drawToForeground();
}

void TileMap::drawToBackground()
{
	graphics.createBackgroundTexture((float)(mapWidth * tileWidth), (float)(mapHeight * tileHeight));

	size_t bgLayerIndex = 0;
	if (bgLayerIndex < tileGridLayers.size())
	{
		std::vector<std::vector<Tile>> layer = tileGridLayers.at(bgLayerIndex);

		for (size_t rowNum = 0; rowNum < layer.size(); rowNum++)
		{
			for (size_t colNum = 0; colNum < layer.at(0).size(); colNum++)
			{
				Tile tile = layer.at(rowNum).at(colNum);
				if (tile.id >= 0 && tile.tileSheetId >= 0)
				{
					Rectangle<float> destRect((float) colNum * tileWidth,
						(float) rowNum * tileHeight,
						(float) tileWidth,
						(float) tileHeight);

					std::string filePath = tileSheetPaths.at(tile.tileSheetId);
					// scaled is false because it's already scaled here
					graphics.drawToBackgroundTexture(filePath, tile.id, destRect, tile.flippedDiagonally, tile.flippedHorizontally, tile.flippedVertically, false);
				}
			}
		}
	}
}

void TileMap::drawToForeground()
{
	graphics.createForegroundTexture((float)(mapWidth * tileWidth), (float)(mapHeight * tileHeight));

	size_t fgLayerIndex = 1;
	if (fgLayerIndex < tileGridLayers.size())
	{
		std::vector<std::vector<Tile>> layer = tileGridLayers.at(fgLayerIndex);

		for (size_t rowNum = 0; rowNum < layer.size(); rowNum++)
		{
			for (size_t colNum = 0; colNum < layer.at(0).size(); colNum++)
			{
				Tile tile = layer.at(rowNum).at(colNum);
				if (tile.id >= 0 && tile.tileSheetId >= 0)
				{
					Rectangle<float> destRect((float)colNum * tileWidth,
						(float)rowNum * tileHeight,
						(float)tileWidth,
						(float)tileHeight);

					std::string filePath = tileSheetPaths.at(tile.tileSheetId);
					// scaled is false because it's already scaled here
					graphics.drawToForegroundTexture(filePath, tile.id, destRect, tile.flippedDiagonally, tile.flippedHorizontally, tile.flippedVertically, false);
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

int TileMap::getHeightInTiles() const
{
	return mapHeight;
}

int TileMap::getWidthInTiles() const
{
	return mapWidth;
}

int TileMap::getTileHeight() const
{
	return tileHeight;
}

int TileMap::getTileWidth() const
{
	return tileWidth;
}

const std::vector<std::vector<Rectangle<float>>>& TileMap::getCollisionGrid() const
{
	return collisionGrid;
}