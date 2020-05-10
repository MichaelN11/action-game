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
		graphics.loadTileSheet(tileSet.source, tileSet.tileWidth, tileSet.tileHeight, tileSet.spacing);
		tileSheetPaths.push_back(tileSet.source);
	}

	mapWidth = data.mapWidth;
	mapHeight = data.mapHeight;
	tileWidth = data.tileWidth;
	tileHeight = data.tileHeight;
}

void TileMap::draw(Graphics& graphics)
{
	int rowNum, colNum;
	for (std::vector<std::vector<Tile>> layer : tileGridLayers)
	{
		rowNum = 0;
		for (std::vector<Tile> row : layer)
		{
			colNum = 0;
			for (Tile tile : row)
			{
				if (tile.id >= 0 && tile.tileSheetId >= 0)
				{
					Rectangle destRect((int)(colNum * tileWidth * graphics.getScale()),
						(int)(rowNum * tileHeight * graphics.getScale()),
						tileWidth,
						tileHeight);

					std::string filePath = tileSheetPaths.at(tile.tileSheetId);
					graphics.drawImage(filePath, tile.id, destRect, tile.flippedDiagonally, tile.flippedHorizontally, tile.flippedVertically, true);
				}

				colNum++;
			}
			rowNum++;
		}
	}
}